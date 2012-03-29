import unittest
import msgpackrpc

class TestKyotoTycoonMsgPack(unittest.TestCase):
  def setUp(self):
    self._address = msgpackrpc.Address('localhost', 18801)
    self._client = msgpackrpc.Client(self._address, unpack_encoding='utf-8')
  def tearDown(self):
    self._client.close()
  def test_void(self):
    result = self._client.call('void')
    self.assertTrue(result)
  def test_echo(self):
    args = { 'hoge': 1, 'foo': 'bar' }
    result = self._client.call('echo', args)
    self.assertEquals(result, args)

if __name__ == '__main__':
  unittest.main()
