#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include "network.h"
#include "user.h"

using namespace std;

SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);
    displayLoginPage();

    string userfile = "/Users/msun/Desktop/Winter\ 2024/CSCI62/SocialNetworkGUI/users.txt";
    char *userfileCStr = userfile.data();

    string postsfile = "/Users/msun/Desktop/Winter\ 2024/CSCI62/SocialNetworkGUI/posts.txt";
    char *postfileCStr = postsfile.data();

    if(net.readUsers(userfileCStr) == 0 && net.readPosts(postfileCStr) == 0) {
        cout << "Success reading in files";
    } else {
        cout << "Error reading in files";
    }

    connect(ui->loginButton, &QPushButton::clicked, this, &SocialNetworkWindow::loginButtonCallback);
    connect(ui->backButton, &QPushButton::clicked, this, &SocialNetworkWindow::backButtonCallback);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::addButtonCallback);

    connect(ui->friendsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::friendTableCallback);
    connect(ui->friendSuggestionsTable, &QTableWidget::cellClicked, this, &SocialNetworkWindow::suggestionTableCallback);
}

void SocialNetworkWindow::loginButtonCallback(){

    string name = ui->loginEdit->toPlainText().toStdString();

    int id = net.getId(name);

    if(id == -1) {
        ui->errorLabel->show();
    }else {
        setLoggedUser(id);
        displayProfilePage();
    }
    ui->loginEdit->clear();
}

void SocialNetworkWindow::addButtonCallback(){
    loggedUser->addFriend(currentUser->getId());
}

void SocialNetworkWindow::addSuggestedCallback() {

    QWidget *w = qobject_cast<QWidget *>(sender()->parent());
    int row = 0;
    bool flag = 1;

    if(w){
        row = ui->friendSuggestionsTable->indexAt(w->pos()).row();
        flag = ui->friendSuggestionsTable->item(row,0)->text().isNull();
    }

    QTableWidgetItem* item = ui->friendSuggestionsTable->item(row, 0);
    QString userString = item->text();
    int id = std::stoi(userString.toStdString());
    loggedUser->addFriend(id);

    if(!flag) {
        ui->friendSuggestionsTable->removeRow(row);
        ui->friendSuggestionsTable->setCurrentCell(0, 0);
    }
    //if logged is being displayed then update table
    if(currentUser->getId() == loggedUser->getId()) {
        setFriendTable(loggedUser->getId());
    }
}

void SocialNetworkWindow::backButtonCallback() {
    showUser(loggedUser->getId());
    net.writeUsers("/Users/msun/Desktop/Winter\ 2024/CSCI62/SocialNetworkGUI/users.txt");
}

void SocialNetworkWindow::friendTableCallback(int row, int column) {

    QTableWidgetItem* user = ui->friendsTable->item(row, column);

    if (column == 1) {
        user = ui->friendsTable->item(row, 0);
    }

    //convert qtablewidgetitme to int
    QString userString = user->text();
    int id = std::stoi(userString.toStdString());

    showUser(id);
}

void SocialNetworkWindow::suggestionTableCallback(int row, int column) {

    QTableWidgetItem* user = ui->friendSuggestionsTable->item(row, column);

    if (column == 1) {
        user = ui->friendSuggestionsTable->item(row, 0);
    }

    //convert qtablewidgetitme to int
    QString userString = user->text();
    int id = std::stoi(userString.toStdString());

    showUser(id);
}

void SocialNetworkWindow::setLoggedUser(int id) {
    loggedUser = net.getUser(id);
    currentUser = net.getUser(id);
    showUser(id);
}

void SocialNetworkWindow::showUser(int id) {
    //showUser handles friend table, friend suggestion table, and posts.
    currentUser = net.getUser(id);

    if(currentUser->getId() == loggedUser->getId()) {
        ui->profileLabel->setText(QString::fromStdString("My Profile"));
        ui->addFriendButton->hide();
        ui->friendSuggestionsLabel->show();
        ui->friendSuggestionsTable->show();
        setFriendSuggestions(id);
    } else {
        ui->profileLabel->setText(QString::fromStdString(currentUser->getName()));
        ui->addFriendButton->show();
        ui->friendSuggestionsLabel->hide();
        ui->friendSuggestionsTable->hide();
    }

    setFriendTable(id);

    setPosts(id);
}

void SocialNetworkWindow::setFriendTable(int id) {

    User* u = net.getUser(id);
    set<int> friendSet = u->getFriends();

    int numRows = friendSet.size();
    int numColumns = 2;

    ui->friendsTable->setRowCount(numRows);
    ui->friendsTable->setColumnCount(numColumns);

    vector<string> names;
    vector<int> ids;

    for(auto friend_: friendSet) {

        User* u = net.getUser(friend_);
        string name = u->getName();
        int id = u->getId();

        names.push_back(name);
        ids.push_back(id);
    }

    for(int row = 0; row < numRows; row++) {

        QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(names[row]));
        QTableWidgetItem *id = new QTableWidgetItem(QString::number(ids[row]));
        ui->friendsTable->setColumnWidth(0, 40);
        ui->friendsTable->setColumnWidth(1, 200);
        ui->friendsTable->setHorizontalHeaderLabels({"ID","Name"});
        ui->friendsTable->setItem(row, 1, name);
        ui->friendsTable->setItem(row, 0, id);
    }

}

void SocialNetworkWindow::setFriendSuggestions(int id) {

    int score;
    vector<int> friendSet = net.suggestFriends(id, score);

    int numRows = friendSet.size();


    ui->friendSuggestionsTable->setRowCount(numRows);
    ui->friendSuggestionsTable->setColumnCount(3);

    vector<string> names;
    vector<int> ids;

    for(auto friend_: friendSet) {

        User* u = net.getUser(friend_);
        string name = u->getName();
        int id = u->getId();

        names.push_back(name);
        ids.push_back(id);
    }
    //construct the table
    for(int row = 0; row < numRows; row++) {

        QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(names[row]));
        QTableWidgetItem *id = new QTableWidgetItem(QString::number(ids[row]));
        QPushButton *addFriendButton = new QPushButton("Add Friend");

        //style
        ui->friendSuggestionsTable->setColumnWidth(0, 40);
        ui->friendSuggestionsTable->setColumnWidth(1, 120);
        ui->friendSuggestionsTable->setColumnWidth(2, 100);

        ui->friendSuggestionsTable->setHorizontalHeaderLabels({"ID","Name"});
        ui->friendSuggestionsTable->setItem(row, 1, name);
        ui->friendSuggestionsTable->setItem(row, 0, id);
        ui->friendSuggestionsTable->setCellWidget(row, 2, addFriendButton);

        connect(addFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::addSuggestedCallback);
    }

    ui->friendSuggestionsTable->setHorizontalHeaderLabels({"Id", "Suggested Friend","Add as Friend?"});

}

void SocialNetworkWindow::displayLoginPage() {
    ui->errorLabel->hide();
    ui->loginLabel->show();
    ui->loginButton->show();
    ui->loginEdit->show();
    ui->profileLabel->hide();
    ui->friendsTable->hide();
    ui->friendsLabel->hide();
    ui->postsLabel->hide();
    ui->backButton->hide();
    ui->addFriendButton->hide();
    ui->friendSuggestionsTable->hide();
    ui->friendSuggestionsLabel->hide();
    ui->post->hide();
}

void SocialNetworkWindow::displayProfilePage() {
    ui->errorLabel->hide();
    ui->loginLabel->hide();
    ui->loginButton->hide();
    ui->loginEdit->hide();
    ui->profileLabel->show();
    ui->friendsTable->show();
    ui->friendsLabel->show();
    ui->postsLabel->show();
    ui->backButton->show();
    ui->addFriendButton->show();
    ui->friendSuggestionsTable->show();
    ui->friendSuggestionsLabel->show();
    ui->post->show();
}

bool isWhitespace(unsigned char c) {
    if (c == '\t' || c == '\r' || c == '\f' || c == '\v') {
        return true;
    } else {
        return false;
    }
}

void SocialNetworkWindow::setPosts(int id) {

    string posts = net.getPostsString(id, 5, 0);
    posts.erase(std::remove_if(posts.begin(), posts.end(), isWhitespace), posts.end());
    ui->post->setText(QString::fromStdString(posts));
}

SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}
