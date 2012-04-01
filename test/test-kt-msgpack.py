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
    # normal
    self._client.call('set', 'get1', 'get1')
    ret1 = self._client.call('get', 'get1')
    self.assertEqual(ret1, { u'value': u'get1' })

    # expiration
    self._client.call('set', 'get2', 'get2', { 'xt': '100000' })
    ret2 = self._client.call('get', 'get2')
    self.assertEqual(ret2['value'], u'get2')
    self.assertTrue(ret2.has_key('xt'))

    # specific key of no existing record.
    try:
      self._client.call('get', 'xxxx')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35);

    # specific database name.
    self._client.call('set', 'get3', '1', { 'DB': 'casket2.kct' })
    ret3 = self._client.call('get', 'get3', { 'DB': 'casket2.kct' })
    self.assertEqual(ret3, { u'value': u'1' })

    # not exist database name.
    try:
      self._client.call('get', 'xxxx', { 'DB': 'xxxx.kct' })
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # specific no parameter.
    try:
      self._client.call_async('get')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)

  def test_append(self):
    # normal
    ret1 = self._client.call('append', 'append1', 'hoge')
    self.assertIsNone(ret1)
    self._client.call('append', 'append1', 'hoge')
    ret1 = self._client.call('get', 'append1')
    self.assertEqual(ret1, { u'value': u'hogehoge' })

    # specific database name.
    ret2 = self._client.call('append', 'append2', 'hoge', { 'DB': 'casket2.kct' });
    self.assertIsNone(ret2)
    ret2 = self._client.call('get', 'append2', { 'DB': 'casket2.kct' })
    self.assertEqual(ret2, { u'value': u'hoge' })

    # not exist database name.
    try:
      self._client.call('append', 'append3', 'hoge', { 'DB': 'xxxx' });
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # specific expiration time.
    ret3 = self._client.call('append', 'xt_append', '1', { 'xt': '1000' })
    self.assertIsNone(ret3)
    ret3 = self._client.call('get', 'xt_append')
    self.assertEqual(ret3.get('value'), u'1')
    self.assertTrue(ret3.has_key('xt'))

    # specific no parameter.
    try:
      self._client.call('append')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)

  def test_remove(self):
    # normal
    self._client.call('set', 'remove1', 'hoge')
    ret1 = self._client.call('remove', 'remove1')
    self.assertIsNone(ret1)
    try:
      self._client.call('get', 'remove1')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35)

    # specific key of no existing record.
    try:
      self._client.call('remove', 'remove2');
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35)

    # specific database name.
    self._client.call('set', 'remove3', 'hoge', { 'DB': 'casket2.kct' });
    ret2 = self._client.call('remove', 'remove3', { 'DB': 'casket2.kct' })
    self.assertIsNone(ret2)
    try:
      self._client.call('get', 'remove3')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35)

    # not exist database name.
    try:
      self._client.call('remove', 'remove4', { 'DB': 'xxxx' });
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # specific no parameter.
    try:
      self._client.call('remove')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)

  def test_seize(self):
    # normal 
    self._client.call('set', 'seize1', 'seize1')
    ret1 = self._client.call('seize', 'seize1')
    self.assertEqual(ret1, { u'value': u'seize1' })
    try:
      self._client.call('seize', 'seize1')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35);

    # expiration
    self._client.call('set', 'seize2', 'seize2', { 'xt': '100000' })
    ret2 = self._client.call('seize', 'seize2')
    self.assertEqual(ret2['value'], u'seize2')
    self.assertTrue(ret2.has_key('xt'))

    # specific key of no existing record.
    try:
      self._client.call('seize', 'xxxx')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35);

    # specific database name.
    self._client.call('set', 'seize3', '1', { 'DB': 'casket2.kct' })
    ret3 = self._client.call('seize', 'seize3', { 'DB': 'casket2.kct' })
    self.assertEqual(ret3, { u'value': u'1' })

    # not exist database name.
    try:
      self._client.call('seize', 'xxxx', { 'DB': 'xxxx.kct' })
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # specific no parameter.
    try:
      self._client.call('seize')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)

  def test_clear(self):
    # normal
    self._client.call('set', 'clear1', '1')
    self._client.call('set', 'clear2', '1')
    ret1 = self._client.call('clear')
    self.assertIsNone(ret1)
    try:
      self._client.call('get', 'clear1')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35)

    # specific database name.
    self._client.call('set', 'clear1', '1', { 'DB': 'casket2.kct' })
    self._client.call('set', 'clear2', '1', { 'DB': 'casket2.kct' })
    ret2 = self._client.call('clear')
    self.assertIsNone(ret2)
    try:
      self._client.call('get', 'clear2', { 'DB': 'casket2.kct' })
    except error.RPCError as e:
      self.assertEqual(e.args[0], 35)

    # not exist database name.
    try:
      self._client.call('clear', { 'DB': 'xxxxx' })
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

  def test_replace(self):
    # normal
    self._client.call('set', 'replace1', '1111')
    ret1 = self._client.call('replace', 'replace1', '2222')
    self.assertIsNone(ret1)
    ret1 = self._client.call('get', 'replace1')
    self.assertEqual(ret1, { u'value': u'2222' })

    # specific database name.
    self._client.call('append', 'replace2', '1', { 'DB': 'casket2.kct' })
    self._client.call('replace', 'replace2', '2', { 'DB': 'casket2.kct' })
    ret2 = self._client.call('get', 'replace2', { 'DB': 'casket2.kct' })
    self.assertEqual(ret2, { u'value': u'2' })

    # not exist database name.
    try:
      self._client.call('replace', 'replace3', 'hgoe', { 'DB': 'xxxxx' })
    except error.RPCError as e:
      self.assertEqual(e.args[0], 34)

    # expiration
    self._client.call('set', 'replace4', 'foo')
    self._client.call('replace', 'replace4', 'bar', { 'xt': '10000' })
    ret4 = self._client.call('get', 'replace4')
    self.assertEqual(ret4.get('value'), u'bar')
    self.assertTrue(ret4.has_key('xt'))

    # specific no parameter.
    try:
      self._client.call('replace')
    except error.RPCError as e:
      self.assertEqual(e.args[0], 2)

if __name__ == '__main__':
  unittest.main()
