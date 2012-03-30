#ifndef MPRPC_KyotoTycoonService_client_0e54fd99_HPP__
#define MPRPC_KyotoTycoonService_client_0e54fd99_HPP__

#include "KyotoTycoonService.hpp"


namespace KyotoTycoonService {


class client : public msgpack::rpc::client::base {
public:
	client(const msgpack::rpc::address& addr, msgpack::rpc::loop lo = msgpack::rpc::loop()) :
		msgpack::rpc::client::base(addr, lo) { }

	client(const std::string& host, uint16_t port, msgpack::rpc::loop lo = msgpack::rpc::loop()) :
		msgpack::rpc::client::base(host, port, lo) { }

	~client() { }

	bool ping_apply(
			const KyotoTycoonService::ping& message) {
		return instance.call_apply("ping", message).get<bool>();
	}

	bool ping(
			) {
		KyotoTycoonService::ping _Message;
		return ping_apply(_Message);
	}

	msgpack::rpc::future::type<bool> ping_async_apply(
			const KyotoTycoonService::ping& message) {
		return instance.call_apply("ping", message);
	}

	msgpack::rpc::future::type<bool> ping_async(
			) {
		KyotoTycoonService::ping _Message;
		return ping_async_apply(_Message);
	}
	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  echo_apply(
			const KyotoTycoonService::echo& message) {
		return instance.call_apply("echo", message).get<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> >();
	}

	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  echo(
			const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::echo _Message;
		_Message.inmap = inmap;
		return echo_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > echo_async_apply(
			const KyotoTycoonService::echo& message) {
		return instance.call_apply("echo", message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > echo_async(
			const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::echo _Message;
		_Message.inmap = inmap;
		return echo_async_apply(_Message);
	}
	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  report_apply(
			const KyotoTycoonService::report& message) {
		return instance.call_apply("report", message).get<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> >();
	}

	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  report(
			) {
		KyotoTycoonService::report _Message;
		return report_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > report_async_apply(
			const KyotoTycoonService::report& message) {
		return instance.call_apply("report", message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > report_async(
			) {
		KyotoTycoonService::report _Message;
		return report_async_apply(_Message);
	}
};


}  // namespace KyotoTycoonService


#endif
