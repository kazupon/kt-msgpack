#ifndef MPRPC_KyotoTycoonService_client_72afcf7b_HPP__
#define MPRPC_KyotoTycoonService_client_72afcf7b_HPP__

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
	std::map<std::string,std::string>  echo_apply(
			const KyotoTycoonService::echo& message) {
		return instance.call_apply("echo", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  echo(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::echo _Message;
		_Message.inmap = inmap;
		return echo_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > echo_async_apply(
			const KyotoTycoonService::echo& message) {
		return instance.call_apply("echo", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > echo_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::echo _Message;
		_Message.inmap = inmap;
		return echo_async_apply(_Message);
	}
	std::map<std::string,std::string>  report_apply(
			const KyotoTycoonService::report& message) {
		return instance.call_apply("report", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  report(
			) {
		KyotoTycoonService::report _Message;
		return report_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > report_async_apply(
			const KyotoTycoonService::report& message) {
		return instance.call_apply("report", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > report_async(
			) {
		KyotoTycoonService::report _Message;
		return report_async_apply(_Message);
	}
	std::map<std::string,std::string>  status_apply(
			const KyotoTycoonService::status& message) {
		return instance.call_apply("status", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  status(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::status _Message;
		_Message.inmap = inmap;
		return status_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > status_async_apply(
			const KyotoTycoonService::status& message) {
		return instance.call_apply("status", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > status_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::status _Message;
		_Message.inmap = inmap;
		return status_async_apply(_Message);
	}
	void add_apply(
			const KyotoTycoonService::add& message) {
		instance.call_apply("add", message).get<void>();
	}

	void add(
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
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
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
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
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
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
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::set _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return set_async_apply(_Message);
	}
	std::map<std::string,std::string>  get_apply(
			const KyotoTycoonService::get& message) {
		return instance.call_apply("get", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  get(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::get _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return get_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > get_async_apply(
			const KyotoTycoonService::get& message) {
		return instance.call_apply("get", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > get_async(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::get _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return get_async_apply(_Message);
	}
	void remove_apply(
			const KyotoTycoonService::remove& message) {
		instance.call_apply("remove", message).get<void>();
	}

	void remove(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::remove _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return remove_apply(_Message);
	}

	msgpack::rpc::future::type<void> remove_async_apply(
			const KyotoTycoonService::remove& message) {
		return instance.call_apply("remove", message);
	}

	msgpack::rpc::future::type<void> remove_async(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::remove _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return remove_async_apply(_Message);
	}
	void append_apply(
			const KyotoTycoonService::append& message) {
		instance.call_apply("append", message).get<void>();
	}

	void append(
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::append _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return append_apply(_Message);
	}

	msgpack::rpc::future::type<void> append_async_apply(
			const KyotoTycoonService::append& message) {
		return instance.call_apply("append", message);
	}

	msgpack::rpc::future::type<void> append_async(
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::append _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return append_async_apply(_Message);
	}
	std::map<std::string,std::string>  seize_apply(
			const KyotoTycoonService::seize& message) {
		return instance.call_apply("seize", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  seize(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::seize _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return seize_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > seize_async_apply(
			const KyotoTycoonService::seize& message) {
		return instance.call_apply("seize", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > seize_async(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::seize _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return seize_async_apply(_Message);
	}
};


}  // namespace KyotoTycoonService


#endif
