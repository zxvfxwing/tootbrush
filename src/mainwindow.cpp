#include "include/mainwindow.h"
#include "include/request.h"
#include "include/mastodon.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    instance_url = new QLineEdit(this);
    instance_url->setText("https://");

    create_menu();

    QVBoxLayout* main_layout = new QVBoxLayout;
    build_main_layout(main_layout);

    QPushButton* test = new QPushButton(this);
    test->setText("Connection");
    connect(test, &QPushButton::clicked, this, &MainWindow::http_request);

    main_layout->addWidget(test);

    main = new QWidget(this);
    main->setLayout(main_layout);

    setCentralWidget(main);
    setMinimumSize(600, 500);
    setWindowTitle("tootbrush 0.1");
}

MainWindow::~MainWindow()
{}

void
MainWindow::build_main_layout(QBoxLayout* main_layout)
{
    main_layout->setMenuBar(menu_bar);
    main_layout->addWidget(instance_url);
}

void
MainWindow::create_menu()
{
    menu_bar = new QMenuBar(this);

    menu_file = new QMenu(tr("File"), this);
    action_exit = menu_file->addAction(tr("Quit"));

    menu_bar->addMenu(menu_file);
    connect(action_exit, &QAction::triggered, this, &QApplication::quit);
}

void
MainWindow::http_request()
{
    QUrl mastodon_api_v1_apps("https://mastodon.xyz" + API::V1::Apps);

    QUrlQuery datas;
    datas.addQueryItem("client_name", "tootbrush");
    datas.addQueryItem("redirect_uris", "urn:ietf:wg:oauth:2.0:oob");
    datas.addQueryItem("scopes", "read:accounts read:statuses write:statuses");
    datas.addQueryItem("website", "https://spokonline.net");

    HttpClient* client = new HttpClient(this);

    client->ping_host(mastodon_api_v1_apps.host());
    client->set_header("User-Agent", "tootbrush")
          ->set_header("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");

    QByteArray answer;
    if( client->POST(mastodon_api_v1_apps, datas, answer) )
        std::cerr << answer.toStdString() << std::endl;

    //QJsonDocument json = QJsonDocument::fromJson(answer);
    QFile json("test.json");
    json.open(QFile::WriteOnly);
    json.write(answer);
}
