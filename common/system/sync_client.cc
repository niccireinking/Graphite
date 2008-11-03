#include "sync_client.h"
#include "network.h"
#include "core.h"
#include "packetize.h"
#include "mcp.h"

#include <iostream>

using namespace std;

SyncClient::SyncClient(Core *core)
  : _core(core)
  , _network(core->getNetwork())
{
}

SyncClient::~SyncClient()
{
}

void SyncClient::mutexInit(int commid, carbon_mutex_t *mux)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_MUTEX_INIT;

  _send_buff << msg_type << commid;

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(carbon_mutex_t) );

  *mux = *((carbon_mutex_t*)res_buff);
  //  _recv_buff << make_pair(res_buff, length);
  //  _recv_buff >> *mux;
}

void SyncClient::mutexLock(int commid, carbon_mutex_t *mux)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_MUTEX_LOCK;

  _send_buff << msg_type << commid << *mux << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(unsigned int) + sizeof(UInt64) );

  unsigned int dummy;
  UInt64 time;
  _recv_buff << make_pair(res_buff, length);
  _recv_buff >> dummy;
  assert( dummy == MUTEX_LOCK_RESPONSE );

  _recv_buff >> time;
  _core->updateProcTime(time);
  
  delete [] res_buff;
}

void SyncClient::mutexUnlock(int commid, carbon_mutex_t *mux)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_MUTEX_UNLOCK;

  _send_buff << msg_type << commid << *mux << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(unsigned int) );

  unsigned int dummy;
  _recv_buff << make_pair(res_buff, length);
  _recv_buff >> dummy;
  assert( dummy == MUTEX_UNLOCK_RESPONSE );

  delete [] res_buff;
}

void SyncClient::condInit(int commid, carbon_cond_t *cond)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_COND_INIT;

  _send_buff << msg_type << commid << *cond << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(carbon_cond_t) );

  *cond = *((carbon_cond_t*)res_buff);

  delete [] res_buff;
}

void SyncClient::condWait(int commid, carbon_cond_t *cond, carbon_mutex_t *mux)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_COND_WAIT;

  _send_buff << msg_type << commid << *cond << *mux << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(unsigned int) + sizeof(UInt64) );

  unsigned int dummy;
  _recv_buff << make_pair(res_buff, length);
  _recv_buff >> dummy;
  assert( dummy == COND_WAIT_RESPONSE );

  UInt64 time;
  _recv_buff >> time;
  _core->updateProcTime(time);

  delete [] res_buff;
}

void SyncClient::condSignal(int commid, carbon_cond_t *cond)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_COND_SIGNAL;

  _send_buff << msg_type << commid << *cond << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(unsigned int) );

  unsigned int dummy;
  _recv_buff << make_pair(res_buff, length);
  _recv_buff >> dummy;
  assert( dummy == COND_SIGNAL_RESPONSE );

  delete [] res_buff;
}

void SyncClient::condBroadcast(int commid, carbon_cond_t *cond)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_COND_BROADCAST;

  _send_buff << msg_type << commid << *cond << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(unsigned int) );

  unsigned int dummy;
  _recv_buff << make_pair(res_buff, length);
  _recv_buff >> dummy;
  assert( dummy == COND_BROADCAST_RESPONSE );

  delete [] res_buff;
}

void SyncClient::barrierInit(int commid, carbon_barrier_t *barrier, UINT32 count)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_BARRIER_INIT;

  _send_buff << msg_type << commid << count << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(carbon_barrier_t) );

  *barrier = *((carbon_barrier_t*)res_buff);

  delete [] res_buff;
}

void SyncClient::barrierWait(int commid, carbon_barrier_t *barrier)
{
  // Reset the buffers for the new transmission
  _recv_buff.clear(); 
  _send_buff.clear(); 
   
  int msg_type = MCP_MESSAGE_BARRIER_WAIT;

  _send_buff << msg_type << commid << *barrier << _core->getProcTime();

  _network->getTransport()->ptSendToMCP((UInt8 *) _send_buff.getBuffer(), _send_buff.size());

  UInt32 length = 0;
  UInt8 *res_buff = _network->getTransport()->ptRecvFromMCP(&length);
  assert( length == sizeof(unsigned int) + sizeof(UInt64) );

  unsigned int dummy;
  _recv_buff << make_pair(res_buff, length);
  _recv_buff >> dummy;
  assert( dummy == BARRIER_WAIT_RESPONSE );

  UInt64 time;
  _recv_buff >> time;
  _core->updateProcTime(time);

  delete [] res_buff;
}

