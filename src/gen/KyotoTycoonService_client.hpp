#ifndef MPRPC_KyotoTycoonService_client_f9109899_HPP__
#define MPRPC_KyotoTycoonService_client_f9109899_HPP__

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
	void clear_apply(
			const KyotoTycoonService::clear& message) {
		instance.call_apply("clear", message).get<void>();
	}

	void clear(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::clear _Message;
		_Message.inmap = inmap;
		return clear_apply(_Message);
	}

	msgpack::rpc::future::type<void> clear_async_apply(
			const KyotoTycoonService::clear& message) {
		return instance.call_apply("clear", message);
	}

	msgpack::rpc::future::type<void> clear_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::clear _Message;
		_Message.inmap = inmap;
		return clear_async_apply(_Message);
	}
	void replace_apply(
			const KyotoTycoonService::replace& message) {
		instance.call_apply("replace", message).get<void>();
	}

	void replace(
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::replace _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return replace_apply(_Message);
	}

	msgpack::rpc::future::type<void> replace_async_apply(
			const KyotoTycoonService::replace& message) {
		return instance.call_apply("replace", message);
	}

	msgpack::rpc::future::type<void> replace_async(
			const std::string& key, const std::string& value, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::replace _Message;
		_Message.key = key;
		_Message.value = value;
		_Message.inmap = inmap;
		return replace_async_apply(_Message);
	}
	void cas_apply(
			const KyotoTycoonService::cas& message) {
		instance.call_apply("cas", message).get<void>();
	}

	void cas(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cas _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return cas_apply(_Message);
	}

	msgpack::rpc::future::type<void> cas_async_apply(
			const KyotoTycoonService::cas& message) {
		return instance.call_apply("cas", message);
	}

	msgpack::rpc::future::type<void> cas_async(
			const std::string& key, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cas _Message;
		_Message.key = key;
		_Message.inmap = inmap;
		return cas_async_apply(_Message);
	}
	std::map<std::string,std::string>  increment_apply(
			const KyotoTycoonService::increment& message) {
		return instance.call_apply("increment", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  increment(
			const std::string& key, const std::string& num, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::increment _Message;
		_Message.key = key;
		_Message.num = num;
		_Message.inmap = inmap;
		return increment_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > increment_async_apply(
			const KyotoTycoonService::increment& message) {
		return instance.call_apply("increment", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > increment_async(
			const std::string& key, const std::string& num, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::increment _Message;
		_Message.key = key;
		_Message.num = num;
		_Message.inmap = inmap;
		return increment_async_apply(_Message);
	}
	std::map<std::string,std::string>  increment_double_apply(
			const KyotoTycoonService::increment_double& message) {
		return instance.call_apply("increment_double", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  increment_double(
			const std::string& key, const std::string& num, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::increment_double _Message;
		_Message.key = key;
		_Message.num = num;
		_Message.inmap = inmap;
		return increment_double_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > increment_double_async_apply(
			const KyotoTycoonService::increment_double& message) {
		return instance.call_apply("increment_double", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > increment_double_async(
			const std::string& key, const std::string& num, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::increment_double _Message;
		_Message.key = key;
		_Message.num = num;
		_Message.inmap = inmap;
		return increment_double_async_apply(_Message);
	}
	std::map<std::string,std::string>  match_prefix_apply(
			const KyotoTycoonService::match_prefix& message) {
		return instance.call_apply("match_prefix", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  match_prefix(
			const std::string& prefix, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::match_prefix _Message;
		_Message.prefix = prefix;
		_Message.inmap = inmap;
		return match_prefix_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > match_prefix_async_apply(
			const KyotoTycoonService::match_prefix& message) {
		return instance.call_apply("match_prefix", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > match_prefix_async(
			const std::string& prefix, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::match_prefix _Message;
		_Message.prefix = prefix;
		_Message.inmap = inmap;
		return match_prefix_async_apply(_Message);
	}
	std::map<std::string,std::string>  match_regex_apply(
			const KyotoTycoonService::match_regex& message) {
		return instance.call_apply("match_regex", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  match_regex(
			const std::string& regex, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::match_regex _Message;
		_Message.regex = regex;
		_Message.inmap = inmap;
		return match_regex_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > match_regex_async_apply(
			const KyotoTycoonService::match_regex& message) {
		return instance.call_apply("match_regex", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > match_regex_async(
			const std::string& regex, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::match_regex _Message;
		_Message.regex = regex;
		_Message.inmap = inmap;
		return match_regex_async_apply(_Message);
	}
	std::map<std::string,std::string>  set_bulk_apply(
			const KyotoTycoonService::set_bulk& message) {
		return instance.call_apply("set_bulk", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  set_bulk(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::set_bulk _Message;
		_Message.inmap = inmap;
		return set_bulk_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > set_bulk_async_apply(
			const KyotoTycoonService::set_bulk& message) {
		return instance.call_apply("set_bulk", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > set_bulk_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::set_bulk _Message;
		_Message.inmap = inmap;
		return set_bulk_async_apply(_Message);
	}
	std::map<std::string,std::string>  remove_bulk_apply(
			const KyotoTycoonService::remove_bulk& message) {
		return instance.call_apply("remove_bulk", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  remove_bulk(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::remove_bulk _Message;
		_Message.inmap = inmap;
		return remove_bulk_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > remove_bulk_async_apply(
			const KyotoTycoonService::remove_bulk& message) {
		return instance.call_apply("remove_bulk", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > remove_bulk_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::remove_bulk _Message;
		_Message.inmap = inmap;
		return remove_bulk_async_apply(_Message);
	}
	std::map<std::string,std::string>  get_bulk_apply(
			const KyotoTycoonService::get_bulk& message) {
		return instance.call_apply("get_bulk", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  get_bulk(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::get_bulk _Message;
		_Message.inmap = inmap;
		return get_bulk_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > get_bulk_async_apply(
			const KyotoTycoonService::get_bulk& message) {
		return instance.call_apply("get_bulk", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > get_bulk_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::get_bulk _Message;
		_Message.inmap = inmap;
		return get_bulk_async_apply(_Message);
	}
};


}  // namespace KyotoTycoonService


#endif
