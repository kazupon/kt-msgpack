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
    result = self._client.call('ping')
    self.assertTrue(result)

  def test_echo(self):
    args = { 'hoge': '1', 'foo': 'bar' }
    result = self._client.call('echo', args)
    self.assertEquals(result, args)
    args = {}
    result = self._client.call('echo', args)
    self.assertEquals(result, args)

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

if __name__ == '__main__':
  unittest.main()
