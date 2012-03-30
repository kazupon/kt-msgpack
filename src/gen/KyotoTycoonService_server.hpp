#ifndef MPRPC_KyotoTycoonService_server_1cbbd1da_HPP__
#define MPRPC_KyotoTycoonService_server_1cbbd1da_HPP__

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
	virtual void ping(msgpack::rpc::request::type<bool>, KyotoTycoonService::ping&) = 0;
	virtual void echo(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> >, KyotoTycoonService::echo&) = 0;
	virtual void report(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> >, KyotoTycoonService::report&) = 0;
};


}  // namespace KyotoTycoonService


/*
void KyotoTycoonService::server::ping(msgpack::rpc::request::type<bool> req, KyotoTycoonService::ping& params)
void KyotoTycoonService::server::echo(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > req, KyotoTycoonService::echo& params)
void KyotoTycoonService::server::report(msgpack::rpc::request::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > req, KyotoTycoonService::report& params)
*/


#endif
