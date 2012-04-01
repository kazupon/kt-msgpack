#ifndef MPRPC_KyotoTycoonService_server_adae3d83_HPP__
#define MPRPC_KyotoTycoonService_server_adae3d83_HPP__

#include "KyotoTycoonService.hpp"


namespace KyotoTycoonService {


class server : public msgpack::rpc::server::base {
public:
	server(msgpack::rpc::loop lo = msgpack::rpc::loop()) :
		msgpack::rpc::server::base(lo) { }

	virtual ~server() { }

	void dispatch(msgpack::rpc::request req);

private:
	class dispatch_table {
	public:
		dispatch_table();
		~dispatch_table();
		void* pimpl;
	};

	static dispatch_table s_dispatch_table;
	friend class dispatch_table;

public:
	virtual void ping(msgpack::rpc::request::type<void>, KyotoTycoonService::ping&) = 0;
	virtual void echo(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::echo&) = 0;
	virtual void report(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::report&) = 0;
	virtual void status(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::status&) = 0;
	virtual void add(msgpack::rpc::request::type<void>, KyotoTycoonService::add&) = 0;
	virtual void set(msgpack::rpc::request::type<void>, KyotoTycoonService::set&) = 0;
	virtual void get(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::get&) = 0;
	virtual void remove(msgpack::rpc::request::type<void>, KyotoTycoonService::remove&) = 0;
	virtual void append(msgpack::rpc::request::type<void>, KyotoTycoonService::append&) = 0;
	virtual void seize(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::seize&) = 0;
	virtual void clear(msgpack::rpc::request::type<void>, KyotoTycoonService::clear&) = 0;
	virtual void replace(msgpack::rpc::request::type<void>, KyotoTycoonService::replace&) = 0;
	virtual void cas(msgpack::rpc::request::type<void>, KyotoTycoonService::cas&) = 0;
	virtual void increment(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::increment&) = 0;
	virtual void increment_double(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::increment_double&) = 0;
	virtual void match_prefix(msgpack::rpc::request::type<std::map<std::string,std::string> >, KyotoTycoonService::match_prefix&) = 0;
};


}  // namespace KyotoTycoonService


/*
void KyotoTycoonService::server::ping(msgpack::rpc::request::type<void> req, KyotoTycoonService::ping& params)
void KyotoTycoonService::server::echo(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::echo& params)
void KyotoTycoonService::server::report(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::report& params)
void KyotoTycoonService::server::status(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::status& params)
void KyotoTycoonService::server::add(msgpack::rpc::request::type<void> req, KyotoTycoonService::add& params)
void KyotoTycoonService::server::set(msgpack::rpc::request::type<void> req, KyotoTycoonService::set& params)
void KyotoTycoonService::server::get(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::get& params)
void KyotoTycoonService::server::remove(msgpack::rpc::request::type<void> req, KyotoTycoonService::remove& params)
void KyotoTycoonService::server::append(msgpack::rpc::request::type<void> req, KyotoTycoonService::append& params)
void KyotoTycoonService::server::seize(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::seize& params)
void KyotoTycoonService::server::clear(msgpack::rpc::request::type<void> req, KyotoTycoonService::clear& params)
void KyotoTycoonService::server::replace(msgpack::rpc::request::type<void> req, KyotoTycoonService::replace& params)
void KyotoTycoonService::server::cas(msgpack::rpc::request::type<void> req, KyotoTycoonService::cas& params)
void KyotoTycoonService::server::increment(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::increment& params)
void KyotoTycoonService::server::increment_double(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::increment_double& params)
void KyotoTycoonService::server::match_prefix(msgpack::rpc::request::type<std::map<std::string,std::string> > req, KyotoTycoonService::match_prefix& params)
*/


#endif
