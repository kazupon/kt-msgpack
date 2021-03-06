
#include "KyotoTycoonService_server.hpp"
#include <memory>
#include <mp/unordered_map.h>


namespace KyotoTycoonService {


static void dispatch_ping(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::ping message;
	preq->params().convert(&message);
	svr->ping(*preq, message);
}
static void dispatch_echo(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::echo message;
	preq->params().convert(&message);
	svr->echo(*preq, message);
}
static void dispatch_report(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::report message;
	preq->params().convert(&message);
	svr->report(*preq, message);
}
static void dispatch_status(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::status message;
	preq->params().convert(&message);
	svr->status(*preq, message);
}
static void dispatch_add(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::add message;
	preq->params().convert(&message);
	svr->add(*preq, message);
}
static void dispatch_set(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::set message;
	preq->params().convert(&message);
	svr->set(*preq, message);
}
static void dispatch_get(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::get message;
	preq->params().convert(&message);
	svr->get(*preq, message);
}
static void dispatch_remove(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::remove message;
	preq->params().convert(&message);
	svr->remove(*preq, message);
}
static void dispatch_append(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::append message;
	preq->params().convert(&message);
	svr->append(*preq, message);
}
static void dispatch_seize(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::seize message;
	preq->params().convert(&message);
	svr->seize(*preq, message);
}
static void dispatch_clear(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::clear message;
	preq->params().convert(&message);
	svr->clear(*preq, message);
}
static void dispatch_replace(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::replace message;
	preq->params().convert(&message);
	svr->replace(*preq, message);
}
static void dispatch_cas(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cas message;
	preq->params().convert(&message);
	svr->cas(*preq, message);
}
static void dispatch_increment(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::increment message;
	preq->params().convert(&message);
	svr->increment(*preq, message);
}
static void dispatch_increment_double(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::increment_double message;
	preq->params().convert(&message);
	svr->increment_double(*preq, message);
}
static void dispatch_match_prefix(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::match_prefix message;
	preq->params().convert(&message);
	svr->match_prefix(*preq, message);
}
static void dispatch_match_regex(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::match_regex message;
	preq->params().convert(&message);
	svr->match_regex(*preq, message);
}
static void dispatch_set_bulk(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::set_bulk message;
	preq->params().convert(&message);
	svr->set_bulk(*preq, message);
}
static void dispatch_remove_bulk(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::remove_bulk message;
	preq->params().convert(&message);
	svr->remove_bulk(*preq, message);
}
static void dispatch_get_bulk(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::get_bulk message;
	preq->params().convert(&message);
	svr->get_bulk(*preq, message);
}
static void dispatch_vacuum(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::vacuum message;
	preq->params().convert(&message);
	svr->vacuum(*preq, message);
}
static void dispatch_synchronize(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::synchronize message;
	preq->params().convert(&message);
	svr->synchronize(*preq, message);
}
static void dispatch_play_script(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::play_script message;
	preq->params().convert(&message);
	svr->play_script(*preq, message);
}
static void dispatch_tune_replication(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::tune_replication message;
	preq->params().convert(&message);
	svr->tune_replication(*preq, message);
}
static void dispatch_cur_jump(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_jump message;
	preq->params().convert(&message);
	svr->cur_jump(*preq, message);
}
static void dispatch_cur_jump_back(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_jump_back message;
	preq->params().convert(&message);
	svr->cur_jump_back(*preq, message);
}
static void dispatch_cur_step(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_step message;
	preq->params().convert(&message);
	svr->cur_step(*preq, message);
}
static void dispatch_cur_step_back(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_step_back message;
	preq->params().convert(&message);
	svr->cur_step_back(*preq, message);
}
static void dispatch_cur_set_value(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_set_value message;
	preq->params().convert(&message);
	svr->cur_set_value(*preq, message);
}
static void dispatch_cur_remove(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_remove message;
	preq->params().convert(&message);
	svr->cur_remove(*preq, message);
}
static void dispatch_cur_get_key(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_get_key message;
	preq->params().convert(&message);
	svr->cur_get_key(*preq, message);
}
static void dispatch_cur_get_value(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_get_value message;
	preq->params().convert(&message);
	svr->cur_get_value(*preq, message);
}
static void dispatch_cur_get(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_get message;
	preq->params().convert(&message);
	svr->cur_get(*preq, message);
}
static void dispatch_cur_seize(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_seize message;
	preq->params().convert(&message);
	svr->cur_seize(*preq, message);
}
static void dispatch_cur_delete(server* svr, msgpack::rpc::request* preq)
{
	KyotoTycoonService::cur_delete message;
	preq->params().convert(&message);
	svr->cur_delete(*preq, message);
}

typedef mp::unordered_map<std::string, void (*)(server*, msgpack::rpc::request*)> table_type;
#define TABLE server::s_dispatch_table.pimpl
server::dispatch_table server::s_dispatch_table;

server::dispatch_table::dispatch_table()
{
	std::auto_ptr<table_type> table(new table_type());
	table->insert(std::make_pair("ping", &dispatch_ping));
	table->insert(std::make_pair("echo", &dispatch_echo));
	table->insert(std::make_pair("report", &dispatch_report));
	table->insert(std::make_pair("status", &dispatch_status));
	table->insert(std::make_pair("add", &dispatch_add));
	table->insert(std::make_pair("set", &dispatch_set));
	table->insert(std::make_pair("get", &dispatch_get));
	table->insert(std::make_pair("remove", &dispatch_remove));
	table->insert(std::make_pair("append", &dispatch_append));
	table->insert(std::make_pair("seize", &dispatch_seize));
	table->insert(std::make_pair("clear", &dispatch_clear));
	table->insert(std::make_pair("replace", &dispatch_replace));
	table->insert(std::make_pair("cas", &dispatch_cas));
	table->insert(std::make_pair("increment", &dispatch_increment));
	table->insert(std::make_pair("increment_double", &dispatch_increment_double));
	table->insert(std::make_pair("match_prefix", &dispatch_match_prefix));
	table->insert(std::make_pair("match_regex", &dispatch_match_regex));
	table->insert(std::make_pair("set_bulk", &dispatch_set_bulk));
	table->insert(std::make_pair("remove_bulk", &dispatch_remove_bulk));
	table->insert(std::make_pair("get_bulk", &dispatch_get_bulk));
	table->insert(std::make_pair("vacuum", &dispatch_vacuum));
	table->insert(std::make_pair("synchronize", &dispatch_synchronize));
	table->insert(std::make_pair("play_script", &dispatch_play_script));
	table->insert(std::make_pair("tune_replication", &dispatch_tune_replication));
	table->insert(std::make_pair("cur_jump", &dispatch_cur_jump));
	table->insert(std::make_pair("cur_jump_back", &dispatch_cur_jump_back));
	table->insert(std::make_pair("cur_step", &dispatch_cur_step));
	table->insert(std::make_pair("cur_step_back", &dispatch_cur_step_back));
	table->insert(std::make_pair("cur_set_value", &dispatch_cur_set_value));
	table->insert(std::make_pair("cur_remove", &dispatch_cur_remove));
	table->insert(std::make_pair("cur_get_key", &dispatch_cur_get_key));
	table->insert(std::make_pair("cur_get_value", &dispatch_cur_get_value));
	table->insert(std::make_pair("cur_get", &dispatch_cur_get));
	table->insert(std::make_pair("cur_seize", &dispatch_cur_seize));
	table->insert(std::make_pair("cur_delete", &dispatch_cur_delete));
	TABLE = (void*)table.release();
}

server::dispatch_table::~dispatch_table()
{
	delete (table_type*)TABLE;
}

void server::dispatch(msgpack::rpc::request req)
try {
	std::string method;
	req.method().convert(&method);

	const table_type* table((table_type*)TABLE);

	table_type::const_iterator m = table->find(method);
	if(m == table->end()) {
		req.error(msgpack::rpc::NO_METHOD_ERROR);
		return;
	}

	(*m->second)(this, &req);

} catch (msgpack::type_error& e) {
	req.error(msgpack::rpc::ARGUMENT_ERROR);
	return;

} catch (std::exception& e) {
	req.error(std::string(e.what()));
	return;
}


}  // namespace KyotoTycoonService

