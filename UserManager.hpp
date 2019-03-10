#pragma once 

#include<iostream>
#include<string>
#include<unordered_map>
#include<pthread.h>

class User{
    private:
	std::string nick_name;
	std::string school;
	std::string passwd;
    public:
	User(const std::string &n_,const std::string &school_,\
		const std::string pwd_)
		:nick_name(n_)
		,school(school_)
		,passwd(pwd_)
	{}
	
	bool IsPasswdOk(const std::string &passwd_)
	{
	    return passwd == passwd_ ? true:false;
	}
	
	~User()
	{}
};

class UserManager{
    private:
	unsigned int assign_id;
	std::unordered_map<unsigned int,User>users;
	std::unordered_map<unsigned int,struct sockaddr_in>online_users;
	pthread_mutex_t lock;
	void Lock()
	{
	    pthread_mutex_lock(&lock);
	}
	void Unlock()
	{
	    pthread_mutex_unlock(&lock);
	}
    public:
	UserManager():assign_id(10000)
	{
	    pthread_mutex_init(&lock,NULL);
	}

	unsigned int Insert(const std::string &n_,\
			    const std::string &s_,\
			    const std::string &pwd)
	{
	    Lock();
	    unsigned int id = assign_id++;
	    User u(n_,s_,pwd);
	    if(users.find(id) == users.end()){
		//users.insert(make_pair(id,u));
		users.insert({id,u});
		Unlock();
		return id;
	    }
	    Unlock();
	    return 1;
	}
	unsigned int Check(const unsigned int &id,const std::string &passwd)
	{
	    Lock();
	    auto it = users.find(id);
	    if(it != users.end()){
		User &u = it->second;
		if(u.IsPasswdOk(passwd)){
		   Unlock();
		   return id;
		}
	    }
	    Unlock();
	    return 2;
	}
	void AddOnlineUser(const unsigned int& id,struct sockaddr_in& peer)
	{
	    Lock();
	    auto it = online_users.find(id);
	    if(it == online_users.end()){
		online_users.insert({id,peer});
	    }
	    Unlock();
	}
	std::unordered_map<unsigned int ,struct sockaddr_in> OnlineUser()
	{
	    Lock();
	    auto online = online_users;
	    Unlock();
	    return online;
	}
	~UserManager()
	{
	    pthread_mutex_destroy(&lock);
	}
};
























