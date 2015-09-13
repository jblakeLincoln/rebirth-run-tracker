#include "MainWindow.h"
#include "ui_MainWindow.h"

QString logFilePath = "C:\\Users\\The Gatekeeper\\Documents\\My Games\\Binding of Isaac Rebirth\\log.txt";
QString outFilePath = "C:\\Users\\The Gatekeeper\\Documents\\My Games\\Binding of Isaac Rebirth\\mylog.txt";
QString exeFilePath = "E:\\Steam\\SteamApps\\common\\The Binding of Isaac Rebirth\\isaac-ng.exe";
QString exeFilePath = "E:\\Program Files (x86)\\Steam\\SteamApps\\common\\The Binding of Isaac Rebirth\\isaac-ng.exe";


MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_LineCount = 0;

    // Load in fonts
    int id = QFontDatabase::addApplicationFont(":/resources/font_Upheavtt.ttf");
    fontFamRebirth = QFontDatabase::applicationFontFamilies(id).at(0);

    id = QFontDatabase::addApplicationFont(":/resources/font_Adore.ttf");
    fontFamPickup = QFontDatabase::applicationFontFamilies(id).at(0);

    // Set up grid for images
    grLayout = new QGridLayout();
    ui->scrollArea->setLayout(grLayout);

    // Set up text labels
    QFont fontSeed;
    fontSeed.setFamily(fontFamRebirth);
    fontSeed.setPixelSize(36);
    ui->lbl_Seed->setFont(fontSeed);

    QFont fontPickup;
    fontPickup.setFamily(fontFamPickup);
    fontPickup.setPixelSize(16);
    fontPickup.setLetterSpacing(QFont::PercentageSpacing, 100);
    ui->lbl_PickupText->setFont(fontPickup);
    ui->lbl_PickupText->setStyleSheet("QLabel { text-align: center; color: grey;}");
    ui->lbl_PickupText->setMaximumWidth(200);

    QFont fontEffect = ui->lbl_EffectText->font();
    fontEffect.setPixelSize(14);
    ui->lbl_EffectText->setFont(fontEffect);


    // Load in items from text file
    InitialiseItems();
    connect(ui->Btn_Launch, SIGNAL(clicked(bool)), this, SLOT(Btn_Launch_Clicked()));
}

void MainWindow::InitialiseItems()
{
    QFile inputFile(":/resources/items.txt");

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        while (!in.atEnd())
        {
            QString line = in.readLine();

            if (line.contains("<id"))
            {

                line = line.trimmed();
                QString strId = line.mid(4);
                int id = strId.left(strId.length()-1).toInt();

                QString name = GetXMLEntry(in.readLine());
                QString type, charge, pools, pickup, effect;

                while (!line.contains("</id>"))
                {
                    if (line.contains("<type>"))
                        type = GetXMLEntry(line);
                    else if (line.contains("<pool>"))
                        pools = GetXMLEntry(line);
                    else if (line.contains("<pickup>"))
                        pickup = GetXMLEntry(line);
                    else if (line.contains("<charge>"))
                        charge = GetXMLEntry(line);
                    else if (line.contains("<effect>"))
                        effect = GetXMLEntry(line);
                    else
                        effect += line;
                     line = in.readLine();
                }

                effect = effect.left(effect.indexOf('<'));
                if (charge == "")
                    charge = "-1";
                _itemMap[id] = RebirthItem(name, type, charge, pools, pickup, effect);
            }
        }
        inputFile.close();
    }
}

QString MainWindow::GetXMLEntry(const QString& str)
{
    QString out = str;
    int start = str.indexOf('>')+1;
    int end = str.indexOf('/')-1;
    return out.mid(start, end-start);
}



void MainWindow::Btn_Launch_Clicked()
{
    ui->Btn_Launch->setEnabled(false);
    m_LineCount = 0;


    process = new QProcess(this);
    QString folder = "E:\\";
    process->start(exeFilePath, QStringList() << folder);

    m_UpdateTimer = new QTimer(this);
    connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(Update()));
    m_UpdateTimer->start(500);

    QFile(outFilePath).remove();
    QFile(logFilePath).remove();
}

void MainWindow::Update()
{
    // If Rebirth has finished running, re-enable the button and exit
    if (process->state() == QProcess::CrashExit)
    {
        m_UpdateTimer->stop();
        ui->Btn_Launch->setEnabled(true);
        return;
    }

    // Update our personal log file
    if (QFile(outFilePath).exists())
        QFile(outFilePath).remove();
    QFile::copy(logFilePath, outFilePath);

    ReadFile();
}

void MainWindow::ReadFile()
{
    QFile inputFile(outFilePath);
    int lineCount = 0;

    bool flush = false;
    bool rerollingAll = false;
    bool rerollingAllCount = -1;
    int currentItem = -1;

    int currentStage = 0;
    bool altStage = 0;
    QString seed;
    QString character;
    QString curse;

    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);

        while (!in.atEnd())
        {
            currentItem = -1;
            QString line = in.readLine();
            if (lineCount++ < m_LineCount)
                continue;

            if (line.contains("Adding collectible"))
            {
                int start = 19;
                int end = line.indexOf('(')-1;
                int id = line.mid(start, end-start).toInt();
                //ui->textBrowser->append(_itemMap.at(id).m_Name);
                //ui->textBrowser->append(_itemMap.at(id).m_PickupText);
                currentItem = id;

                QString name = _itemMap.at(id).GetName();
                ui->lbl_PickupText->setText(_itemMap.at(id).GetPickup());
                ui->lbl_EffectText->setText(_itemMap.at(id).GetEffect());
                ui->lbl_EffectText->adjustSize();
                AddImageToPanel(name);
            }

            if (flush)
            {
                m_HeldItem = currentItem;
                flush = false;
            }
            else if (line.contains("item queue flush"))
            {
                flush = true;
            }

            if (line.contains("Collectibles"))
            {
                rerollingAll = true;
                int start = line.indexOf(' ')+1;
                int end = line.indexOf(' ', start);
                rerollingAllCount = line.mid(start, end-start).toInt();
            }

            if (line.contains("Level::Init m_Stage"))
            {
                currentStage = line.mid(20, 1).toInt();
                altStage = line.mid(34, 1).toInt();
            }

            if (line.contains("RNG Start Seed"))
            {
                seed = line.mid(16, 9);
                ui->lbl_Seed->setText(seed);
            }
            if (line.contains("player with Variant"))
            {
                int index = line.indexOf(' ', 44)+1;
                character = line.mid(index, line.length()-index);
            }

            if (line.contains("Curse of"))
                curse = line;

        }
        m_LineCount = lineCount;
        inputFile.close();
    }

}

void MainWindow::AddImageToPanel(QString name)
{
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
        grLayout->addWidget(new QLabel(), 0+j, 0+i);

    float m = 2;
    name = name.replace("\'", "");
    name = name.replace(' ', '-');
    name = name.replace("/", "-");
    QPixmap pix = QPixmap(":/images/" + name + ".png");
    int w = pix.width();
    int h = pix.height();
    ui->img_LastItem->setPixmap(pix.scaled(QSize(w*3, h*3),Qt::IgnoreAspectRatio));

    QLabel* l = new QLabel();
    l->setPixmap(pix.scaled(QSize(w*m, h*m), Qt::KeepAspectRatio));
    l->setFixedSize(w*m, h*m);
    grLayout->setAlignment(l, Qt::AlignCenter | Qt::AlignTop);
    int c = itemCount % 5;
    int r = itemCount / 5;
    grLayout->addWidget(l, r, c);


    //grLayout->cellRect(r,c).setSize(QSize(64,64));

    itemCount++;
}
