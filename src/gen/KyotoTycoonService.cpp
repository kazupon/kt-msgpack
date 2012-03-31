
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

