import sys
import random
import unittest
import msgpackrpc
from msgpackrpc import error

class TestKyotoTycoonMsgPack(unittest.TestCase):

  def setUp(self):
    self._address = msgpackrpc.Address('127.0.0.1', 18801)
    self._client = msgpackrpc.Client(self._address, unpack_encoding='utf-8')

  def tearDown(self):
    self._client.close()

  def test_ping(self):
    ret = self._client.call('ping')
    self.assertIsNone(ret)

  def test_echo(self):
    # normal
    args1 = { 'hoge': '1', 'foo': 'bar' }
    ret1 = self._client.call('echo', args1)
    self.assertEqual(ret1, args1)

    # empty
    args2 = {}
    ret2 = self._client.call('echo', args2)
    self.assertEqual(ret2, args2)

    # specific no parameter
    ret3 = self._client.call('echo')
    self.assertEqual(ret3, {})

    # typemiss match
    args4 = { 'hoge': 1, 'foo': 0.111 }
    try:
      self._client.call('echo', args4)
      self.assertTrue(False)
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2);

  def test_report(self):
    ret = self._client.call('report')
    reports = [
      'cnt_get',
      'cnt_get_misses',
      'cnt_misc',
      'cnt_remove',
      'cnt_remove_misses',
      'cnt_script',
      'cnt_set',
      'cnt_set_misses',
      'conf_kc_features',
      'conf_kc_version',
      'conf_kt_features',
      'conf_kt_version',
      'conf_os_name',
      'db_0',
      'db_total_count',
      'db_total_size',
      'serv_conn_count',
      'serv_current_time',
      'serv_proc_id',
      'serv_running_term',
      'serv_task_count',
      'serv_thread_count',
      'sys_mem_peak',
      'sys_mem_rss',
      'sys_mem_size',
      'sys_ru_stime',
      'sys_ru_utime'
    ]
    for key in reports:
      self.assertTrue(ret.has_key(key))

  def test_status(self):
    reports = [
      'count',
      'size'
    ]
    ret1 = self._client.call('status')
    for key in reports:
      self.assertTrue(ret1.has_key(key))
    ret2 = self._client.call('status', { 'DB': 'casket2.kct' })
    for key in reports:
      self.assertTrue(ret2.has_key(key))

  def test_add(self):
    add_key = ''.join(['add', str(random.randrange(0, sys.maxint))])

    # normal
    ret1 = self._client.call('add', add_key, 'foo')
    self.assertIsNone(ret1)

    # exist record case.
    try:
      self._client.call('add', add_key, 'foo')
      self.assertTrue(False)
    except error.RPCError as e:
      self.assertEqual(e.args[0], 33);

    # specific database name.
    add_key = ''.join(['add', str(random.randrange(0, sys.maxint))])
    ret3 = self._client.call('add', add_key, '1', { 'DB': 'casket2.kct' })
    self.assertIsNone(ret3)

    # not exist database name.
    try:
      self._client.call('add', 'bar', '1', { 'DB': 'xxxx.kct' })
      self.assertTrue(False)
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34);

    # specific expiration time.
    add_key = ''.join(['add', str(random.randrange(0, sys.maxint))])
    ret4 = self._client.call('add', add_key, '111', { 'xt': '100000' })
    self.assertIsNone(ret4)

    # specific no parameter.
    try:
      self._client.call('add')
      self.assertTrue(False)
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2);

  def test_set(self):
    # normal.
    ret1 = self._client.call('set', 'set1', 'set1')
    self.assertIsNone(ret1);
  
    # specific database name.
    ret2 = self._client.call('set', 'set1', '1', { 'DB': 'casket2.kct' })
    self.assertIsNone(ret2);

    # not exist database name.
    try:
      self._client.call('set', 'bar', '1', { 'DB': 'xxxx.kct' })
      self.assertTrue(False)
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34);

    # specific expiration time.
    ret3 = self._client.call('set', 'xt_set', 'dskfld', { 'xt': '1' })
    self.assertIsNone(ret3);

    # specific no parameter.
    try:
      self._client.call('set')
      self.assertTrue(False)
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2);

  def test_get(self):
    that = self

    # get value
    def set1_result_handler(result):
      ret1 = that._client.call('get', 'get1')
      that.assertEqual(ret1, { u'value': u'get1' })

    f1 = self._client.call_async('set', 'get1', 'get1')
    f1.attach_result_handler(set1_result_handler)

    # get value & xt
    def set2_result_handler(result):
      ret2 = that._client.call('get', 'get2')
      that.assertEqual(ret2['value'], u'get2')
      that.assertTrue(ret2.has_key('xt'))

    f2 = self._client.call_async('set', 'get2', 'get2', { 'xt': '100000' })
    f2.attach_result_handler(set2_result_handler)

    # specific key of no existing record.
    def error_code_35_handler(err):
      that.assertEqual(err.args[0], 35);
    
    f3 = self._client.call_async('get', 'xxxx')
    f3.attach_error_handler(error_code_35_handler)

    # specific database name.
    def set3_result_handler(result):
      ret3 = that._client.call('get', 'get3', { 'DB': 'casket2.kct' })
      that.assertEqual(ret3, { u'value': u'1' })

    f4 = self._client.call_async('set', 'get3', '1', { 'DB': 'casket2.kct' })
    f4.attach_result_handler(set3_result_handler)

    # not exist database name.
    def error_code_34_handler(err):
      that.assertEqual(err.args[0], 34)

    f5 = self._client.call_async('get', 'xxxx', { 'DB': 'xxxx.kct' })
    f5.attach_error_handler(error_code_34_handler)

    # specific no parameter.
    def error_invalid_param_handler(err):
      that.assertEqual(err.args[0], 2)

    f6 = self._client.call_async('get')
    f6.attach_error_handler(error_invalid_param_handler)


if __name__ == '__main__':
  unittest.main()
