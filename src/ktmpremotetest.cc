//#include <ktremotedb.h>
#include "./gen/client.hpp"
#include "cmdcommon.h"


// global variables
const char* g_progname;                  // program name
uint32_t g_randseed;                     // random seed
int64_t g_memusage;                      // memory usage

// function prototypes
int main(int argc, char** argv);
static void usage();
static void dbmetaprint(KyotoTycoonService::client& client, bool verbose);
static int32_t runorder(int argc, char** argv);
static int32_t runbulk(int argc, char** argv);
static int32_t procorder(int64_t rnum, int32_t thnum, bool rnd, int32_t mode, const char* host, int32_t port, double tout);
static int32_t procbulk(int64_t rnum, int32_t thnum, bool rnd, int32_t mode, int32_t bulk, const char* host, int32_t port, double tout, int32_t bopts);


// main routine
int main(int argc, char** argv) {
  g_progname = argv[0];
  const char* ebuf = kc::getenv("KTRNDSEED");
  g_randseed = ebuf ? (uint32_t)kc::atoi(ebuf) : (uint32_t)(kc::time() * 1000);
  mysrand(g_randseed);
  g_memusage = memusage();
  kc::setstdiobin();
  if (argc < 2) usage();
  int32_t rv = 0;
  if (!std::strcmp(argv[1], "order")) {
    rv = runorder(argc, argv);
  } else if (!std::strcmp(argv[1], "bulk")) {
    rv = runbulk(argc, argv);
  } else {
    usage();
  }
  if (rv != 0) {
    oprintf("FAILED: KTRNDSEED=%u PID=%ld", g_randseed, (long)kc::getpid());
    for (int32_t i = 0; i < argc; i++) {
      oprintf(" %s", argv[i]);
    }
    oprintf("\n\n");
  }
  return rv;
}


// print the usage and exit
static void usage() {
  eprintf("%s: test cases of msgpack rpc for Kyoto Tycoon\n", g_progname);
  eprintf("\n");
  eprintf("usage:\n");
  eprintf("  %s order [-th num] [-rnd] [-set|-get|-rem|-etc]"
          " [-host str] [-port num] [-tout num] rnum\n", g_progname);
  eprintf("  %s bulk [-th num] [-rnd] [-set|-get|-rem|-etc] [-bulk num]"
          " [-host str] [-port num] [-tout num] [-bnr] rnum\n", g_progname);
  eprintf("\n");
  std::exit(1);
}


// print members of a database
static void dbmetaprint(KyotoTycoonService::client& client) {
  std::map<std::string, std::string> inmap;
  std::map<std::string, std::string> status;
  try {
    status = client.status(inmap);
    std::map<std::string, std::string>::const_iterator it = status.find("count");
    if (it != status.end()) {
      oprintf("%s: %s\n", it->first.c_str(), it->second.c_str());
    }
    it = status.find("size");
    if (it != status.end()) {
      oprintf("%s: %s\n", it->first.c_str(), it->second.c_str());
    }
  } catch (msgpack::rpc::remote_error& e) {
    oprintf("status error: %d", e.error());
  }
  int64_t musage = memusage();
  if (musage > 0) {
    oprintf("memory: %lld\n", (long long)(musage - g_memusage));
  }
}


// parse arguments of order command
static int32_t runorder(int argc, char** argv) {
  bool argbrk = false;
  const char* rstr = NULL;
  int32_t thnum = 1;
  bool rnd = false;
  int32_t mode = 0;
  const char* host = "localhost";
  int32_t port = 18801;
  double tout = 0;
  for (int32_t i = 2; i < argc; i++) {
    if (!argbrk && argv[i][0] == '-') {
      if (!std::strcmp(argv[i], "--")) {
        argbrk = true;
      } else if (!std::strcmp(argv[i], "-th")) {
        if (++i >= argc) usage();
        thnum = kc::atoix(argv[i]);
      } else if (!std::strcmp(argv[i], "-rnd")) {
        rnd = true;
      } else if (!std::strcmp(argv[i], "-set")) {
        mode = 's';
      } else if (!std::strcmp(argv[i], "-get")) {
        mode = 'g';
      } else if (!std::strcmp(argv[i], "-rem")) {
        mode = 'r';
      } else if (!std::strcmp(argv[i], "-etc")) {
        mode = 'e';
      } else if (!std::strcmp(argv[i], "-host")) {
        if (++i >= argc) usage();
        host = argv[i];
      } else if (!std::strcmp(argv[i], "-port")) {
        if (++i >= argc) usage();
        port = kc::atoi(argv[i]);
      } else if (!std::strcmp(argv[i], "-tout")) {
        if (++i >= argc) usage();
        tout = kc::atof(argv[i]);
      } else {
        usage();
      }
    } else if (!rstr) {
      argbrk = false;
      rstr = argv[i];
    } else {
      usage();
    }
  }
  if (!rstr) usage();
  int64_t rnum = kc::atoix(rstr);
  if (rnum < 1 || thnum < 1 || port < 1) usage();
  if (thnum > THREADMAX) thnum = THREADMAX;
  int32_t rv = procorder(rnum, thnum, rnd, mode, host, port, tout);
  return rv;
}


// parse arguments of bulk command
static int32_t runbulk(int argc, char** argv) {
  bool argbrk = false;
  const char* rstr = NULL;
  int32_t thnum = 1;
  bool rnd = false;
  int32_t mode = 0;
  int32_t bulk = 1;
  const char* host = "localhost";
  int32_t port = 18801;
  double tout = 0;
  int32_t bopts = 0;
  for (int32_t i = 2; i < argc; i++) {
    if (!argbrk && argv[i][0] == '-') {
      if (!std::strcmp(argv[i], "--")) {
        argbrk = true;
      } else if (!std::strcmp(argv[i], "-th")) {
        if (++i >= argc) usage();
        thnum = kc::atoix(argv[i]);
      } else if (!std::strcmp(argv[i], "-rnd")) {
        rnd = true;
      } else if (!std::strcmp(argv[i], "-set")) {
        mode = 's';
      } else if (!std::strcmp(argv[i], "-get")) {
        mode = 'g';
      } else if (!std::strcmp(argv[i], "-rem")) {
        mode = 'r';
      } else if (!std::strcmp(argv[i], "-etc")) {
        mode = 'e';
      } else if (!std::strcmp(argv[i], "-bulk")) {
        if (++i >= argc) usage();
        bulk = kc::atoi(argv[i]);
      } else if (!std::strcmp(argv[i], "-host")) {
        if (++i >= argc) usage();
        host = argv[i];
      } else if (!std::strcmp(argv[i], "-port")) {
        if (++i >= argc) usage();
        port = kc::atoi(argv[i]);
      } else if (!std::strcmp(argv[i], "-tout")) {
        if (++i >= argc) usage();
        tout = kc::atof(argv[i]);
      } else if (!std::strcmp(argv[i], "-bnr")) {
        bopts |= kt::RemoteDB::BONOREPLY;
      } else {
        usage();
      }
    } else if (!rstr) {
      argbrk = false;
      rstr = argv[i];
    } else {
      usage();
    }
  }
  if (!rstr) usage();
  int64_t rnum = kc::atoix(rstr);
  if (rnum < 1 || thnum < 1 || bulk < 1 || port < 1) usage();
  if (thnum > THREADMAX) thnum = THREADMAX;
  int32_t rv = procbulk(rnum, thnum, rnd, mode, bulk, host, port, tout, bopts);
  return rv;
}


// perform order command
static int32_t procorder(int64_t rnum, int32_t thnum, bool rnd, int32_t mode,
                         const char* host, int32_t port, double tout) {
  oprintf("<In-order Test>\n  seed=%u  rnum=%lld  thnum=%d  rnd=%d  mode=%d  host=%s  port=%d"
          "  tout=%f\n\n", g_randseed, (long long)rnum, thnum, rnd, mode, host, port, tout);
  bool err = false;
  double stime, etime;
  KyotoTycoonService::client client(std::string(host), port);
  if (mode != 'g' && mode != 'r') {
    std::map<std::string, std::string> inmap;
    try {
      client.clear(inmap);
    } catch (msgpack::rpc::remote_error& e) {
      oprintf("clear error: %d", e.error());
      err = true;
    }
  }
  if (mode == 0 || mode == 's' || mode == 'e') {
    oprintf("setting records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          int64_t xt = rnd_ ? myrand(600) + 1 : kc::INT64MAX;
          std::map<std::string, std::string> inmap;
          std::string str_xt;
          kc::strprintf(&str_xt, "%ld", (long long)xt);
          inmap.insert(std::make_pair("xt", str_xt));
          try {
            client.set(std::string(kbuf, ksiz), std::string(kbuf, ksiz), inmap);
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("set error: %d", e.error());
            err_ = true;
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) oprintf(" (%08lld)\n", (long long)i);
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool rnd_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  if (mode == 'e') {
    oprintf("adding records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          int64_t xt = rnd_ ? myrand(600) + 1 : kc::INT64MAX;
          std::map<std::string, std::string> inmap;
          std::string str_xt;
          kc::strprintf(&str_xt, "%ld", (long long)xt);
          inmap.insert(std::make_pair("xt", str_xt));
          try {
            client.add(std::string(kbuf, ksiz), std::string(kbuf, ksiz), inmap);
          } catch (msgpack::rpc::remote_error& e) {
            if (e.error() != 33) {
              oprintf("add error: %d", e.error());
              err_ = true;
            }
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) oprintf(" (%08lld)\n", (long long)i);
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool rnd_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  if (mode == 'e') {
    oprintf("appending records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          int64_t xt = rnd_ ? myrand(600) + 1 : kc::INT64MAX;
          std::map<std::string, std::string> inmap;
          std::string str_xt;
          kc::strprintf(&str_xt, "%ld", (long long)xt);
          inmap.insert(std::make_pair("xt", str_xt));
          try {
            client.append(std::string(kbuf, ksiz), std::string(kbuf, ksiz), inmap);
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("append error: %d", e.error());
            err_ = true;
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) oprintf(" (%08lld)\n", (long long)i);
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool rnd_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  if (mode == 0 || mode == 'g' || mode == 'e') {
    oprintf("geting records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          std::map<std::string, std::string> inmap;
          std::map<std::string, std::string> outmap;
          try {
            outmap = client.get(std::string(kbuf, ksiz), inmap);
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("get error: %d", e.error());
            err_ = true;
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) oprintf(" (%08lld)\n", (long long)i);
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool rnd_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  if (mode == 0 || mode == 'r' || mode == 'e') {
    oprintf("removing records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          std::map<std::string, std::string> inmap;
          try {
            client.remove(std::string(kbuf, ksiz), inmap);
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("remove error: %d", e.error());
            err_ = true;
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) oprintf(" (%08lld)\n", (long long)i);
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool rnd_;
      int32_t mode_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  oprintf("%s\n\n", err ? "error" : "ok");
  return err ? 1 : 0;
}


// perform bulk command
static int32_t procbulk(int64_t rnum, int32_t thnum, bool rnd, int32_t mode, int32_t bulk, const char* host, int32_t port, double tout, int32_t bopts) {
  oprintf("<Bulk Test>\n  seed=%u  rnum=%lld  thnum=%d  rnd=%d  mode=%d  bulk=%d"
          "  host=%s  port=%d  tout=%f  bopts=%d\n\n",
          g_randseed, (long long)rnum, thnum, rnd, mode, bulk, host, port, tout, bopts);
  bool err = false;
  double stime, etime;
  KyotoTycoonService::client client(std::string(host), port);
  if (mode != 'g' && mode != 'r') {
    std::map<std::string, std::string> inmap;
    try {
      client.clear(inmap);
    } catch (msgpack::rpc::remote_error& e) {
      oprintf("clear error: %d", e.error());
      err = true;
    }
  }
  if (mode == 0 || mode == 's') {
    oprintf("setting records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), bulk_(0), bopts_(0), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, int32_t bulk, int32_t bopts, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        bulk_ = bulk;
        bopts_ = bopts;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        std::map<std::string, std::string> inmap;
        std::map<std::string, std::string> outmap;
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "_%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          std::string key(kbuf, ksiz);
          inmap[key] = key;
          int64_t xt = rnd_ ? myrand(600) + 1 : kc::INT64MAX;
          std::string str_xt;
          kc::strprintf(&str_xt, "%ld", (long long)xt);
          inmap["xt"] = str_xt;
          if (inmap.size() - 1 >= (size_t)bulk_) {
            try {
              outmap = client.set_bulk(inmap);
              const char* num_ptr  = kt::strmapget(outmap, "num");
              int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
              if (num != (int64_t)inmap.size() - 1) {
                oprintf("set record number unmatch !!\n");
                err_ = true;
              }
              inmap.clear();
            } catch (msgpack::rpc::remote_error& e) {
              oprintf("set_bulk error: %d", e.error());
              err_ = true;
            }
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) {
              oprintf(" (%08lld)\n", (long long)i);
            }
          }
        }
        if (inmap.size() > 0) {
          int64_t xt = rnd_ ? myrand(600) + 1 : kc::INT64MAX;
          std::string str_xt;
          kc::strprintf(&str_xt, "%ld", (long long)xt);
          inmap["xt"] = str_xt;
          try {
            outmap = client.set_bulk(inmap);
            const char* num_ptr  = kt::strmapget(outmap, "num");
            int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
            if (num != (int64_t)inmap.size()) {
              oprintf("set record number unmatch !!");
              err_ = true;
            }
            inmap.clear();
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("set_bulk error: %d", e.error());
            err_ = true;
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool bin_;
      bool rnd_;
      int32_t bulk_;
      int32_t bopts_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, bulk, bopts, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  if (mode == 0 || mode == 'g') {
    oprintf("getting records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), bulk_(0), bopts_(0), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, int32_t bulk, int32_t bopts, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        bulk_ = bulk;
        bopts_ = bopts;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        std::map<std::string, std::string> inmap;
        std::map<std::string, std::string> outmap;
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "_%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          std::string key(kbuf, ksiz);
          inmap[key] = key;
          if (inmap.size() >= (size_t)bulk_) {
            try {
              outmap = client.get_bulk(inmap);
              const char* num_ptr  = kt::strmapget(outmap, "num");
              int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
              if (num < 0) {
                oprintf("get record number unmatch !!\n");
                err_ = true;
              }
              inmap.clear();
            } catch (msgpack::rpc::remote_error& e) {
              oprintf("get_bulk error: %d", e.error());
              err_ = true;
            }
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) {
              oprintf(" (%08lld)\n", (long long)i);
            }
          }
        }
        if (inmap.size() > 0) {
          try {
            outmap = client.get_bulk(inmap);
            const char* num_ptr  = kt::strmapget(outmap, "num");
            int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
            if (num < 0) {
              oprintf("get record number unmatch !!");
              err_ = true;
            }
            inmap.clear();
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("get_bulk error: %d", e.error());
            err_ = true;
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool bin_;
      bool rnd_;
      int32_t bulk_;
      int32_t bopts_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, bulk, bopts, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  if (mode == 0 || mode == 'r') {
    oprintf("removing records:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), bulk_(0), bopts_(0), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, int32_t bulk, int32_t bopts, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        bulk_ = bulk;
        bopts_ = bopts;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        int64_t base = id_ * rnum_;
        int64_t range = rnum_ * thnum_;
        KyotoTycoonService::client client(host_, port_);
        std::map<std::string, std::string> inmap;
        std::map<std::string, std::string> outmap;
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "_%08lld", (long long)(rnd_ ? myrand(range) + 1 : base + i));
          std::string key(kbuf, ksiz);
          inmap[key] = key;
          if (inmap.size() >= (size_t)bulk_) {
            try {
              outmap = client.remove_bulk(inmap);
              const char* num_ptr  = kt::strmapget(outmap, "num");
              int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
              if (num < 0) {
                oprintf("remove record number unmatch !!\n");
                err_ = true;
              }
              inmap.clear();
            } catch (msgpack::rpc::remote_error& e) {
              oprintf("remove_bulk error: %d", e.error());
              err_ = true;
            }
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) {
              oprintf(" (%08lld)\n", (long long)i);
            }
          }
        }
        if (inmap.size() > 0) {
          try {
            outmap = client.remove_bulk(inmap);
            const char* num_ptr  = kt::strmapget(outmap, "num");
            int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
            if (num < 0) {
              oprintf("remove record number unmatch !!");
              err_ = true;
            }
            inmap.clear();
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("remove_bulk error: %d", e.error());
            err_ = true;
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool bin_;
      bool rnd_;
      int32_t bulk_;
      int32_t bopts_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, bulk, bopts, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  if (mode == 0 || mode == 'e') {
    oprintf("performing mixed operations:\n");
    stime = kc::time();
    class Worker : public kc::Thread {
     public:
      Worker() : id_(0), rnum_(0), thnum_(0), rnd_(false), bulk_(0), bopts_(0), host_("localhost"), port_(18801), err_(false) {}
      void setparams(int32_t id, int64_t rnum, int32_t thnum, bool rnd, int32_t bulk, int32_t bopts, std::string host, uint16_t port) {
        id_ = id;
        rnum_ = rnum;
        thnum_ = thnum;
        rnd_ = rnd;
        bulk_ = bulk;
        bopts_ = bopts;
        host_ = host;
        port_ = port;
      }
      bool error() {
        return err_;
      }
     private:
      void run() {
        KyotoTycoonService::client client(host_, port_);
        std::map<std::string, std::string> inmap;
        std::map<std::string, std::string> outmap;
        for (int64_t i = 1; !err_ && i <= rnum_; i++) {
          char kbuf[RECBUFSIZ];
          size_t ksiz = std::sprintf(kbuf, "_%08lld", (long long)(rnd_ ? myrand(rnum_) + 1 : i));
          std::string key(kbuf, ksiz);
          inmap[key] = key;
          int64_t xt = rnd_ ? myrand(600) + 1 : kc::INT64MAX;
          std::string str_xt;
          kc::strprintf(&str_xt, "%ld", (long long)xt);
          inmap["xt"] = str_xt;
          int32_t cmd;
          if (rnd_) {
            cmd = myrand(100);
          } else {
            cmd = i + id_;
            cmd = kc::hashmurmur(&cmd, sizeof(cmd)) % 100;
          }
          if (inmap.size() >= (size_t)bulk_) {
            if (cmd < 50) {
              try {
                outmap = client.get_bulk(inmap);
                const char* num_ptr = kt::strmapget(outmap, "num");
                int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
                if (num < 0) {
                  oprintf("get record number unmatch !!\n");
                  err_ = true;
                }
              } catch (msgpack::rpc::remote_error& e) {
                oprintf("get_bulk error: %d", e.error());
                err_ = true;
              }
            } else if (cmd < 90) {
              try {
                outmap = client.set_bulk(inmap);
                const char* num_ptr  = kt::strmapget(outmap, "num");
                int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
                if (num < 0) {
                  oprintf("set record number unmatch !!");
                  err_ = true;
                }
              } catch (msgpack::rpc::remote_error& e) {
                oprintf("set_bulk error: %d", e.error());
                err_ = true;
              }
            } else if (cmd < 98) {
              try {
                outmap = client.remove_bulk(inmap);
                const char* num_ptr  = kt::strmapget(outmap, "num");
                int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
                if (num < 0) {
                  oprintf("remove record number unmatch !!\n");
                  err_ = true;
                }
              } catch (msgpack::rpc::remote_error& e) {
                oprintf("remove_bulk error: %d", e.error());
                err_ = true;
              }
            } else {
              try {
                outmap = client.play_script("echo", inmap);
              } catch (msgpack::rpc::remote_error& e) {
                if (e.error() != 42) {
                  oprintf("play_script error: %d", e.error());
                  err_ = true;
                }
              }
            }
            inmap.clear();
            outmap.clear();
          }
          if (id_ < 1 && rnum_ > 250 && i % (rnum_ / 250) == 0) {
            oputchar('.');
            if (i == rnum_ || i % (rnum_ / 10) == 0) {
              oprintf(" (%08lld)\n", (long long)i);
            }
          }
        }
        if (inmap.size() > 0) {
          int64_t xt = rnd_ ? myrand(600) + 1 : kc::INT64MAX;
          std::string str_xt;
          kc::strprintf(&str_xt, "%ld", (long long)xt);
          inmap["xt"] = str_xt;
          try {
            outmap = client.set_bulk(inmap);
            const char* num_ptr  = kt::strmapget(outmap, "num");
            int64_t num = num_ptr ? kc::atoi(num_ptr) : kc::INT64MAX;
            if (num != (int64_t)inmap.size()) {
              oprintf("set record number unmatch !!");
              err_ = true;
            }
            inmap.clear();
          } catch (msgpack::rpc::remote_error& e) {
            oprintf("set_bulk error: %d", e.error());
            err_ = true;
          }
        }
      }
      int32_t id_;
      int64_t rnum_;
      int32_t thnum_;
      bool bin_;
      bool rnd_;
      int32_t bulk_;
      int32_t bopts_;
      std::string host_;
      uint16_t port_;
      bool err_;
    };
    Worker workers[THREADMAX];
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].setparams(i, rnum, thnum, rnd, bulk, bopts, std::string(host), port);
      workers[i].start();
    }
    for (int32_t i = 0; i < thnum; i++) {
      workers[i].join();
      if (workers[i].error()) err = true;
    }
    etime = kc::time();
    dbmetaprint(client);
    oprintf("time: %.3f\n", etime - stime);
  }
  oprintf("%s\n\n", err ? "error" : "ok");
  return err ? 1 : 0;
}

// END OF FILE
