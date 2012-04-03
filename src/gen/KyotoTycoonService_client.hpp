#ifndef MPRPC_KyotoTycoonService_client_f8a090eb_HPP__
#define MPRPC_KyotoTycoonService_client_f8a090eb_HPP__

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
	void vacuum_apply(
			const KyotoTycoonService::vacuum& message) {
		instance.call_apply("vacuum", message).get<void>();
	}

	void vacuum(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::vacuum _Message;
		_Message.inmap = inmap;
		return vacuum_apply(_Message);
	}

	msgpack::rpc::future::type<void> vacuum_async_apply(
			const KyotoTycoonService::vacuum& message) {
		return instance.call_apply("vacuum", message);
	}

	msgpack::rpc::future::type<void> vacuum_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::vacuum _Message;
		_Message.inmap = inmap;
		return vacuum_async_apply(_Message);
	}
	void synchronize_apply(
			const KyotoTycoonService::synchronize& message) {
		instance.call_apply("synchronize", message).get<void>();
	}

	void synchronize(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::synchronize _Message;
		_Message.inmap = inmap;
		return synchronize_apply(_Message);
	}

	msgpack::rpc::future::type<void> synchronize_async_apply(
			const KyotoTycoonService::synchronize& message) {
		return instance.call_apply("synchronize", message);
	}

	msgpack::rpc::future::type<void> synchronize_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::synchronize _Message;
		_Message.inmap = inmap;
		return synchronize_async_apply(_Message);
	}
	std::map<std::string,std::string>  play_script_apply(
			const KyotoTycoonService::play_script& message) {
		return instance.call_apply("play_script", message).get<std::map<std::string,std::string> >();
	}

	std::map<std::string,std::string>  play_script(
			const std::string& name, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::play_script _Message;
		_Message.name = name;
		_Message.inmap = inmap;
		return play_script_apply(_Message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > play_script_async_apply(
			const KyotoTycoonService::play_script& message) {
		return instance.call_apply("play_script", message);
	}

	msgpack::rpc::future::type<std::map<std::string,std::string> > play_script_async(
			const std::string& name, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::play_script _Message;
		_Message.name = name;
		_Message.inmap = inmap;
		return play_script_async_apply(_Message);
	}
	void tune_replication_apply(
			const KyotoTycoonService::tune_replication& message) {
		instance.call_apply("tune_replication", message).get<void>();
	}

	void tune_replication(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::tune_replication _Message;
		_Message.inmap = inmap;
		return tune_replication_apply(_Message);
	}

	msgpack::rpc::future::type<void> tune_replication_async_apply(
			const KyotoTycoonService::tune_replication& message) {
		return instance.call_apply("tune_replication", message);
	}

	msgpack::rpc::future::type<void> tune_replication_async(
			const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::tune_replication _Message;
		_Message.inmap = inmap;
		return tune_replication_async_apply(_Message);
	}
	void cur_jump_apply(
			const KyotoTycoonService::cur_jump& message) {
		instance.call_apply("cur_jump", message).get<void>();
	}

	void cur_jump(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_jump _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_jump_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_jump_async_apply(
			const KyotoTycoonService::cur_jump& message) {
		return instance.call_apply("cur_jump", message);
	}

	msgpack::rpc::future::type<void> cur_jump_async(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_jump _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_jump_async_apply(_Message);
	}
	void cur_jump_back_apply(
			const KyotoTycoonService::cur_jump_back& message) {
		instance.call_apply("cur_jump_back", message).get<void>();
	}

	void cur_jump_back(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_jump_back _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_jump_back_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_jump_back_async_apply(
			const KyotoTycoonService::cur_jump_back& message) {
		return instance.call_apply("cur_jump_back", message);
	}

	msgpack::rpc::future::type<void> cur_jump_back_async(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_jump_back _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_jump_back_async_apply(_Message);
	}
	void cur_step_apply(
			const KyotoTycoonService::cur_step& message) {
		instance.call_apply("cur_step", message).get<void>();
	}

	void cur_step(
			const std::string& CUR) {
		KyotoTycoonService::cur_step _Message;
		_Message.CUR = CUR;
		return cur_step_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_step_async_apply(
			const KyotoTycoonService::cur_step& message) {
		return instance.call_apply("cur_step", message);
	}

	msgpack::rpc::future::type<void> cur_step_async(
			const std::string& CUR) {
		KyotoTycoonService::cur_step _Message;
		_Message.CUR = CUR;
		return cur_step_async_apply(_Message);
	}
	void cur_step_back_apply(
			const KyotoTycoonService::cur_step_back& message) {
		instance.call_apply("cur_step_back", message).get<void>();
	}

	void cur_step_back(
			const std::string& CUR) {
		KyotoTycoonService::cur_step_back _Message;
		_Message.CUR = CUR;
		return cur_step_back_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_step_back_async_apply(
			const KyotoTycoonService::cur_step_back& message) {
		return instance.call_apply("cur_step_back", message);
	}

	msgpack::rpc::future::type<void> cur_step_back_async(
			const std::string& CUR) {
		KyotoTycoonService::cur_step_back _Message;
		_Message.CUR = CUR;
		return cur_step_back_async_apply(_Message);
	}
	void cur_set_value_apply(
			const KyotoTycoonService::cur_set_value& message) {
		instance.call_apply("cur_set_value", message).get<void>();
	}

	void cur_set_value(
			const std::string& CUR, const std::string& value, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_set_value _Message;
		_Message.CUR = CUR;
		_Message.value = value;
		_Message.inmap = inmap;
		return cur_set_value_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_set_value_async_apply(
			const KyotoTycoonService::cur_set_value& message) {
		return instance.call_apply("cur_set_value", message);
	}

	msgpack::rpc::future::type<void> cur_set_value_async(
			const std::string& CUR, const std::string& value, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_set_value _Message;
		_Message.CUR = CUR;
		_Message.value = value;
		_Message.inmap = inmap;
		return cur_set_value_async_apply(_Message);
	}
	void cur_remove_apply(
			const KyotoTycoonService::cur_remove& message) {
		instance.call_apply("cur_remove", message).get<void>();
	}

	void cur_remove(
			const std::string& CUR) {
		KyotoTycoonService::cur_remove _Message;
		_Message.CUR = CUR;
		return cur_remove_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_remove_async_apply(
			const KyotoTycoonService::cur_remove& message) {
		return instance.call_apply("cur_remove", message);
	}

	msgpack::rpc::future::type<void> cur_remove_async(
			const std::string& CUR) {
		KyotoTycoonService::cur_remove _Message;
		_Message.CUR = CUR;
		return cur_remove_async_apply(_Message);
	}
	void cur_get_key_apply(
			const KyotoTycoonService::cur_get_key& message) {
		instance.call_apply("cur_get_key", message).get<void>();
	}

	void cur_get_key(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_get_key _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_get_key_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_get_key_async_apply(
			const KyotoTycoonService::cur_get_key& message) {
		return instance.call_apply("cur_get_key", message);
	}

	msgpack::rpc::future::type<void> cur_get_key_async(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_get_key _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_get_key_async_apply(_Message);
	}
	void cur_get_value_apply(
			const KyotoTycoonService::cur_get_value& message) {
		instance.call_apply("cur_get_value", message).get<void>();
	}

	void cur_get_value(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_get_value _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_get_value_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_get_value_async_apply(
			const KyotoTycoonService::cur_get_value& message) {
		return instance.call_apply("cur_get_value", message);
	}

	msgpack::rpc::future::type<void> cur_get_value_async(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_get_value _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_get_value_async_apply(_Message);
	}
	void cur_get_apply(
			const KyotoTycoonService::cur_get& message) {
		instance.call_apply("cur_get", message).get<void>();
	}

	void cur_get(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_get _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_get_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_get_async_apply(
			const KyotoTycoonService::cur_get& message) {
		return instance.call_apply("cur_get", message);
	}

	msgpack::rpc::future::type<void> cur_get_async(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_get _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_get_async_apply(_Message);
	}
	void cur_seize_apply(
			const KyotoTycoonService::cur_seize& message) {
		instance.call_apply("cur_seize", message).get<void>();
	}

	void cur_seize(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_seize _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_seize_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_seize_async_apply(
			const KyotoTycoonService::cur_seize& message) {
		return instance.call_apply("cur_seize", message);
	}

	msgpack::rpc::future::type<void> cur_seize_async(
			const std::string& CUR, const std::map<std::string,std::string> & inmap) {
		KyotoTycoonService::cur_seize _Message;
		_Message.CUR = CUR;
		_Message.inmap = inmap;
		return cur_seize_async_apply(_Message);
	}
	void cur_delete_apply(
			const KyotoTycoonService::cur_delete& message) {
		instance.call_apply("cur_delete", message).get<void>();
	}

	void cur_delete(
			const std::string& CUR) {
		KyotoTycoonService::cur_delete _Message;
		_Message.CUR = CUR;
		return cur_delete_apply(_Message);
	}

	msgpack::rpc::future::type<void> cur_delete_async_apply(
			const KyotoTycoonService::cur_delete& message) {
		return instance.call_apply("cur_delete", message);
	}

	msgpack::rpc::future::type<void> cur_delete_async(
			const std::string& CUR) {
		KyotoTycoonService::cur_delete _Message;
		_Message.CUR = CUR;
		return cur_delete_async_apply(_Message);
	}
};


}  // namespace KyotoTycoonService


#endif
