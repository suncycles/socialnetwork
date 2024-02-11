#ifndef POST_H
#define POST_H

#include <iostream>
#include <string>

class Post
{
public:
	Post();

	~Post();
	
	Post(int messageId, int ownerId, std::string message, int likes);
	
	virtual std::string toString();
	
	int getMessageId();
	
	int getOwnerId();
	
	std::string getMessage();
	
	int getLikes();
	
	virtual std::string getAuthor();
	
	virtual bool getIsPublic();
	
private:
	int messageId_;
	int ownerId_;
	std::string message_;
	int likes_;
};

class IncomingPost : public Post
{
public:
	IncomingPost();

	~IncomingPost();
	
	IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author);

	std::string toString();
	
	std::string getAuthor();
	
	bool getIsPublic();

private:
	std::string author_;
	bool isPublic_;
};

#endif