#include <iostream>
#include "user.h"

//Constructors
User::User() : id_(0), year_(0), zip_(0) {}

User::User(int id, std::string name, int year, int zip, std::set<int> friends)
    : id_(id), name_(name), year_(year), zip_(zip), friends_(friends) {}

// getter functions
int User::getId() {return id_;}

std::string User::getName() {return name_;}

int User::getYear() {return year_;}

int User::getZip() {return zip_;}

std::set<int>& User::getFriends() {return friends_;}

void User::addFriend(int id){
    if(id!=id_){
        friends_.insert(id);
    }
}

void User::deleteFriend(int id) {
	friends_.erase(id);	
}

void User::addPost(Post* p) {
	messages_.push_back(p);
}

std::vector<Post*> User::getPosts() {
	return messages_;
}

std::string User::getPostsString(int howMany, bool showOnlyPublic) {

    std::string returnString;

    if(messages_.size() < howMany) {
        howMany = messages_.size();
    }

    for (int i = 0; i < howMany; ++i)
    {
        Post* post = messages_[messages_.size()-1-i];

        if(!showOnlyPublic){
            returnString += post->toString() + "\n\n";
        }else {
        	if(post->getIsPublic()) {
        		returnString += post->toString() + "\n\n";
        	}
        }
    }

    return returnString;
}
