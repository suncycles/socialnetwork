#include "network.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include "post.h"

Network::Network() {
	users_.clear();
}

Network::~Network() {
	users_.clear();
}

void Network::addUser(User* user) {
	users_.push_back(user);
}

int Network::addConnection(std::string s1, std::string s2) {
	int u1 = getId(s1);
    int u2 = getId(s2);

    User* user1 = getUser(u1);
    User* user2 = getUser(u2);

    if (user1 != nullptr && user2 != nullptr) {
        user1->addFriend(u2);
        user2->addFriend(u1);
        return 0; // Success
    } else {
        return -1; // Invalid users
    }
}

int Network::deleteConnection(std::string s1, std::string s2) {
	int u1 = getId(s1);
    int u2 = getId(s2);
    
    User* user1 = getUser(u1);
    User* user2 = getUser(u2);

    if (user1 != nullptr && user2 != nullptr) {
        user1->deleteFriend(u2);
        user2->deleteFriend(u1);
        return 0; // Success
    } else {
        return -1; // Invalid users
    }
}

int Network::getId(std::string name) {
	for (int i = 0; i< users_.size(); i++) {
		if(users_[i] -> getName() == name){
			return i; 
		}
	}
	return -1;
}
//post: returns the id of given name , or -1

int Network::readUsers(char* fname) {
	std::ifstream inputFile(fname);
	int numUsers;

    if (inputFile.is_open()) {
    	inputFile >> numUsers;

        for (int j = 0; j < (numUsers*4); j++) {
	        int id, year, zip;
	        std::string name;
	        std::string friendString;
	        std::set<int> friends;

	        if (!(inputFile >> id)) break;

	        inputFile >> std::ws; // Ignore whitespace
	        std::getline(inputFile, name);
	        inputFile >> year >> std::ws;
	        inputFile >> zip >> std::ws;
	        std::getline(inputFile, friendString);
	        std::istringstream iss(friendString);

	        int i;
	        while (iss >> i) {
	            friends.insert(i);
	        }
	        
	        User* user = new User(id, name, year, zip, friends);
	        addUser(user);
    	}

    	inputFile.close();
    	return 0; // Success
    } else {
    	return -1;
    }
}

int Network::writeUsers(char *fname) {
	std::ofstream outputFile(fname);
    if (!outputFile.is_open()) {
        return -1; // File cannot be created or opened
    }

    // Write the number of users
    outputFile << users_.size() << std::endl;

    // Write user details
    for (User* user : users_) {
        outputFile << user->getId() << std::endl;
        outputFile << "\t" << user->getName() << std::endl;
        outputFile << "\t" << user->getYear() << std::endl;
        outputFile << "\t" << user->getZip() << std::endl;

        for (int friendId : user->getFriends()) {
            outputFile << "\t" << friendId;
        }

        outputFile << std::endl;
    }

    outputFile.close();
    return 0; // Success
}

int Network::numUsers(){
	return users_.size();
}
//post: returns users vector size

//pre: id is given
User* Network::getUser(int id) {
	int i = 0;
	auto it = users_.begin();
	for(it = users_.begin(); it != users_.end(); it++) {
		if(users_[i]->getId() == id) {
			return users_[i];
		}
		i++;
	}
	return nullptr;
}
//post: if matching id is found it returns the user, else returns null

std::vector<int> Network::shortestPath(int from, int to){ 

	std::queue<int> q;
	std::vector<bool> visited (1, 0);
	std::vector<int> parent (1, 0);
	visited[from] = true;

	parent[from] = -1;
	q.push(from);

	while (q.size() > 0) {
		int cur = q.front();
		q.pop();

		for (auto neighbor : users_[cur]->getFriends()) {
			if (!visited[neighbor]) {
				// traverse one by one until the next one is 'to' while setting each one as visted
				visited[neighbor] = true;
				q.push(neighbor);
				parent[neighbor] = cur;
			}

			if(neighbor == to) {
					// if the to is found then re-construct the path and then return it
					std::vector<int> path;
					int temp = to;
					while(temp!= -1) {
						path.push_back(temp);
						temp = parent[temp];
					}
					std::reverse(path.begin(), path.end());
					return path;
			}
		}
	}
	return {};
}

// pre: given source node and a distance to find
std::vector<int> Network::distanceUser(int from, int& to, int distance) {

	std::vector<bool> visited (1, 0);
	std::vector<int> path;
	std::set<int> temp = getUser(from)->getFriends();
	path.push_back(from);
	visited[from] = true;
	
	for (int i = 0; i < distance; ++i)
	{
		int cur = *temp.begin();

		while(visited[cur]){
			cur++;
		} 
		if(!visited[cur]) {
			visited[cur] = true;
			path.push_back(cur);
		}
		if(i==distance-1) {

			to = cur;
		}
		temp = getUser(cur)->getFriends();
	}

	return path;
}
//post: returns a vector of ids being the path taken to user "to"

//pre: source user is given and a container for score
std::vector<int> Network::suggestFriends(int who, int& score) {

	std::set<int> who_friends = getUser(who)->getFriends();
    std::vector<int> friendIDs(0);
    std::vector<int> friendScores(0);
    std::vector<int> returnVector;

    std::set<int> visitedFriends(who_friends);

    for (int friend_ : who_friends) { // inside usere's friends

        std::set<int> f_Friends = getUser(friend_)->getFriends(); 
        int counter = 0;

        for (int f_f : f_Friends) { // inside user's friend's friends

            if (f_f != who && who_friends.count(f_f) == 0 && visitedFriends.count(f_f) == 0) {
                
                std::set<int> f_f_Friends = getUser(f_f)->getFriends();

                for (int f_f_f : f_f_Friends) { // inside user's friend's friend's friends to find matching friends

                    if (f_f_f != who && who_friends.count(f_f_f) == 1) {

                        counter++;
                    }
                }
	 			//each iteration of this loop goes through the potential friends' friends to find matches. 
	 			//if a match is found on the current iteration, counter++
	 			//the loop exits when the end of one friend of friend's list ends, then add the friend of friend's id and counter to each list.

                friendIDs.push_back(f_f);
                friendScores.push_back(counter);
                visitedFriends.insert(f_f);

                counter = 0;
            }
        }
    }

    int highscore = 0;

    for (int i = 0; i < friendScores.size(); ++i) {
    	highscore = (friendScores[i] > highscore) ? friendScores[i] : highscore;
    } // finding the highest score

    for (int i = 0; i < friendScores.size(); ++i) {
        if (friendScores[i] == highscore) {
            returnVector.push_back(friendIDs[i]);
        }
    } // construct the returnable vector

    score = highscore;
    return returnVector;
}
//post: vector of id's is returned and the score is passed into score

void Network::DFS(int source, std::vector<int> &group, std::vector<bool> &visited) {

	std::stack<int> s;
	visited[source] = true;
	s.push(source);
	group.push_back(source);
	// start dfs at source node

	while(s.size() > 0) {
		int cur = s.top();
		s.pop();

		std::set<int> friends = getUser(cur)->getFriends();

		for(auto neighbor : friends) {
			if (!visited[neighbor])
			{
				visited[neighbor] = true;
				s.push(neighbor);
				group.push_back(neighbor);
			}
		}
	}

}

// pre: Network contains users who are friends
std::vector<std::vector<int>> Network::groups() {

	std::vector<std::vector<int>> returnVector;
	std::vector<bool> visited (numUsers(),0);

	for(auto u : users_) {

		if(!visited[u->getId()]) {

			std::vector<int> g;
			DFS(u->getId(), g, visited);
			returnVector.push_back(g);
		}
	} 
	// go through each users' friends and perform a dfs, constructing a group based on who is connected
	return returnVector;

}
//post: constructs a vector of vectors containing groups


void Network::addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic) {
	User* u = getUser(ownerId);
	int messageid = u->getPosts().size();
		if(isIncoming) {
			u->addPost(new IncomingPost(messageid, ownerId, message, likes, isPublic, authorName)); 
		}else {
			u->addPost(new Post(messageid, ownerId, message, likes));
		}
	}

std::string Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic) {
	User* u = getUser(ownerId);
	return u->getPostsString(howMany, showOnlyPublic);
}

int Network::readPosts(char* fname) {
    std::ifstream inputFile(fname);
    int numPosts;

    if (inputFile.is_open()) {
        inputFile >> numPosts;
        inputFile.ignore();  

        for (int j = 0; j < numPosts; j++) {
            std::string messageid, ownerid, likes, message, temp, incomingAuthor;
            std::string isPublic;

            // call in line by line 
            getline(inputFile, messageid);
            getline(inputFile, message);
            getline(inputFile, ownerid);
            getline(inputFile, likes);

            //check if next line is empty
            if (inputFile.peek() == '\n') {
                inputFile.get();  //consume 1 charcacter
                isPublic = "public";
            } else {
                getline(inputFile>>std::ws, isPublic);
            }//if it is, then set isPublic, else, consume newline

            if (inputFile.peek() == '\n') {
                inputFile.get();  
                incomingAuthor = "";
            } else {
                getline(inputFile>>std::ws, incomingAuthor);
            }

            bool public_ = (isPublic == "public");
            bool incoming = !incomingAuthor.empty();

            messageid.erase(std::remove_if(messageid.begin(), messageid.end(), ::isspace), messageid.end());
            ownerid.erase(std::remove_if(ownerid.begin(), ownerid.end(), ::isspace), ownerid.end());
            likes.erase(std::remove_if(likes.begin(), likes.end(), ::isspace), likes.end());

            int messID_int = stoi(messageid);
            int ownID_int = stoi(ownerid);
            int likes_int = stoi(likes);

            addPost(ownID_int, message, likes_int, incoming, incomingAuthor, public_);
        }

        inputFile.close();
        return 0;  // success
    } else {
        return -1;  // invalid file
    }
}


bool postCompareFunction(Post* i, Post* j) {
	return i->getMessageId() < j->getMessageId();
}

int Network::writePosts(char* fname){
	
	std::ofstream outputFile(fname);
	std::vector<Post*> allPosts;

    for (int i = 0; i < users_.size(); ++i)
    {
    	User* u = getUser(i);
    	std::vector <Post*> posts = u->getPosts(); 
    	for (auto it : posts)
    	{
    		allPosts.push_back(it);
    	} // copy all the posts onto stack in vector
    }

    if (outputFile.is_open()) {
    	outputFile << allPosts.size();

    	for (int i = 0; i < allPosts.size(); ++i)
    	{
    		outputFile << allPosts[i]->getMessageId() << std::endl;
	        outputFile << "\t" << allPosts[i]->getMessage() << std::endl;
	        outputFile << "\t" << allPosts[i]->getOwnerId() << std::endl;
	        outputFile << "\t" << allPosts[i]->getLikes() << std::endl;

	        if(allPosts[i]->getIsPublic() && allPosts[i]->getAuthor() != "") {
	        	outputFile << "\t" << "public" << std::endl;
	        }else if(allPosts[i] -> getAuthor() != ""){
	        	outputFile << "\t" << "private" << std::endl;
	        }else {
	        	outputFile << "\n";
	        }

	        outputFile << "\t" << allPosts[i]->getAuthor() << std::endl;
	    }

        std::sort(allPosts.begin(), allPosts.end(), postCompareFunction);;

    	outputFile.close();

    	return 0; //suceces
    } else {
    	return -1; // invalid file
    }
}