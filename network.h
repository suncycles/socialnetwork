#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <string>
#include "user.h"


class Network {
public:
	Network();

	~Network();

	void addUser(User*);

	int addConnection(std::string s1, std::string s2);

	int deleteConnection(std::string s1, std::string s2);

	int getId(std::string name);

	int readUsers(char* fname);

	int writeUsers(char *fname);

	int numUsers();

	User* getUser(int id);

	std::vector<int> shortestPath(int from, int to);

	std::vector<int> distanceUser(int from, int& to, int distance);

	std::vector<int> suggestFriends(int who, int& score);

	std::vector<std::vector<int>> groups();
	
	void addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic);

	std::string getPostsString(int ownerId, int howMany, bool showOnlyPublic);

	int readPosts(char* fname);

	int writePosts(char* fname);

private:
	std::vector<User*> users_;

	void DFS(int source, std::vector<int> &group, std::vector<bool> &visited);
};

#endif
