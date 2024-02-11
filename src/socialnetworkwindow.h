#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include "network.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    void loginButtonCallback();
    void backButtonCallback();
    void addButtonCallback();
    void addSuggestedCallback();
    void friendTableCallback(int row, int column);
    void suggestionTableCallback(int row, int column);

    void setLoggedUser(int id);
    void showUser(int id);
    void setFriendTable(int id);
    void setFriendSuggestions(int id);
    void setPosts(int id);

    void displayLoginPage();
    void displayProfilePage();


private:
    Ui::SocialNetworkWindow *ui;
    Network net;
    User* currentUser;
    User* loggedUser;
};
#endif // SOCIALNETWORKWINDOW_H
