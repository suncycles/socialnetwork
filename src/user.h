#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <set>
#include "post.h"

class User
{
public:
	//Constructor
	User();

	User(int id_, std::string name_, int year_, int zip_, std::set<int> friends_); 

	// getter functions
	int getId();
	std::string getName();
	int getYear();
	int getZip();
	std::set<int> & getFriends();

	void addFriend(int id);
	void deleteFriend(int id);

	void addPost(Post*);
	std::vector<Post*> getPosts();
	std::string getPostsString(int howMany, bool showOnlyPublic); 

	
private:
	int id_;;
	std::string name_;
	int year_;
	int zip_;
	std::set<int> friends_;
	std::vector<Post*> messages_;
};

#endif
