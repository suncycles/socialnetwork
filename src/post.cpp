#include "post.h"
#include <iostream>

// Post Implementations

Post::Post(){}

Post::~Post(){}

Post::Post(int messageId, int ownerId, std::string message, int likes): 
messageId_(messageId), ownerId_(ownerId), message_(message), likes_(likes){}

std::string Post::toString() {
	return message_ + " Liked by " + std::to_string(likes_) + " people.";
}

int Post::getMessageId() {
	return messageId_;
}

int Post::getOwnerId() {
	return ownerId_;
}

std::string Post::getMessage() {
	return message_;
}

int Post::getLikes() {
	return likes_;
}

std::string Post::getAuthor() {
	return "";
}

bool Post::getIsPublic() {
	return 1;
}

// Incoming Post IMplementeations

IncomingPost::IncomingPost(){}

IncomingPost::~IncomingPost(){}

IncomingPost::IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author)
    : Post(messageId, ownerId, message, likes), author_(author), isPublic_(isPublic) {}

std::string IncomingPost::toString() {
	std::string p = "(public)";
	if (!isPublic_)
	{
		p = "(private)";
	}
	return author_ + " wrote: " + p + " " + Post::toString();
}

std::string IncomingPost::getAuthor() {
	return author_;
}

bool IncomingPost::getIsPublic() {
	return isPublic_;
}
