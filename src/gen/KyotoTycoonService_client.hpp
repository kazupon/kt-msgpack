#ifndef MPRPC_KyotoTycoonService_client_05917f4a_HPP__
#define MPRPC_KyotoTycoonService_client_05917f4a_HPP__

#include "KyotoTycoonService.hpp"


namespace KyotoTycoonService {


class client : public msgpack::rpc::client::base {
public:
	client(const msgpack::rpc::address& addr, msgpack::rpc::loop lo = msgpack::rpc::loop()) :
		msgpack::rpc::client::base(addr, lo) { }

	client(const std::string& host, uint16_t port, msgpack::rpc::loop lo = msgpack::rpc::loop()) :
		msgpack::rpc::client::base(host, port, lo) { }

	~client() { }

	void ping_apply(
			const KyotoTycoonService::ping& message) {
		instance.call_apply("ping", message).get<void>();
	}

	void ping(
			) {
		KyotoTycoonService::ping _Message;
		return ping_apply(_Message);
	}

	msgpack::rpc::future::type<void> ping_async_apply(
			const KyotoTycoonService::ping& message) {
		return instance.call_apply("ping", message);
	}

	msgpack::rpc::future::type<void> ping_async(
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
	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  status_apply(
			const KyotoTycoonService::status& message) {
		return instance.call_apply("status", message).get<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> >();
	}

	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  status(
			const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::status _Message;
		_Message.inmap = inmap;
		return status_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > status_async_apply(
			const KyotoTycoonService::status& message) {
		return instance.call_apply("status", message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > status_async(
			const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::status _Message;
		_Message.inmap = inmap;
		return status_async_apply(_Message);
	}
	void add_apply(
			const KyotoTycoonService::add& message) {
		instance.call_apply("add", message).get<void>();
	}

	void add(
			const msgpack::type::raw_ref& key, const msgpack::type::raw_ref& value, const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::add _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return add_apply(_Message);
	}

	msgpack::rpc::future::type<void> add_async_apply(
			const KyotoTycoonService::add& message) {
		return instance.call_apply("add", message);
	}

	msgpack::rpc::future::type<void> add_async(
			const msgpack::type::raw_ref& key, const msgpack::type::raw_ref& value, const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::add _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return add_async_apply(_Message);
	}
	void set_apply(
			const KyotoTycoonService::set& message) {
		instance.call_apply("set", message).get<void>();
	}

	void set(
			const msgpack::type::raw_ref& key, const msgpack::type::raw_ref& value, const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::set _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return set_apply(_Message);
	}

	msgpack::rpc::future::type<void> set_async_apply(
			const KyotoTycoonService::set& message) {
		return instance.call_apply("set", message);
	}

	msgpack::rpc::future::type<void> set_async(
			const msgpack::type::raw_ref& key, const msgpack::type::raw_ref& value, const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::set _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return set_async_apply(_Message);
	}
	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  get_apply(
			const KyotoTycoonService::get& message) {
		return instance.call_apply("get", message).get<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> >();
	}

	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  get(
			const msgpack::type::raw_ref& key, const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::get _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return get_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > get_async_apply(
			const KyotoTycoonService::get& message) {
		return instance.call_apply("get", message);
	}

	msgpack::rpc::future::type<std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> > get_async(
			const msgpack::type::raw_ref& key, const std::map<msgpack::type::raw_ref,msgpack::type::raw_ref> & inmap) {
		KyotoTycoonService::get _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return get_async_apply(_Message);
	}
};


}  // namespace KyotoTycoonService


#endif
