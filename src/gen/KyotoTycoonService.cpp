
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
  printf("dispatch_echo before : type = %p\n", &message.inmap);
	preq->params().convert(&message);
  printf("dispatch_echo after : type = %p\n", &message.inmap);
	svr->echo(*preq, message);
}

typedef mp::unordered_map<std::string, void (*)(server*, msgpack::rpc::request*)> table_type;
#define TABLE server::s_dispatch_table.pimpl
server::dispatch_table server::s_dispatch_table;

server::dispatch_table::dispatch_table()
{
	std::auto_ptr<table_type> table(new table_type());
	table->insert(std::make_pair("ping", &dispatch_ping));
	table->insert(std::make_pair("echo", &dispatch_echo));
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
  printf("type errpr : ");
	req.error(msgpack::rpc::ARGUMENT_ERROR);
	return;

} catch (std::exception& e) {
  printf("errpr : %s", std::string(e.what()));
	req.error(std::string(e.what()));
	return;
}


}  // namespace KyotoTycoonService

