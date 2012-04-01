
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

