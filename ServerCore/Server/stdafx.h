// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define MAX_USER 100
#define SERVER_PORT 9000

#include "../KomorisLib/ServerLibrary.h"

// TODO: reference additional headers your program requires here

#include "../KomorisLib/CriticalSection.h"
#include "../KomorisLib/MultiThreadSync.h"
#include "../KomorisLib/MemoryPool.h"
#include "../KomorisLib/CircularQueue.h"
#include "../KomorisLib/NetworkSession.h"
#include "../KomorisLib/PacketSession.h"
#include "../KomorisLib/Iocp.h"
#include "../KomorisLib/Stream.h"
#include "../KomorisLib/MiniDump.h"

#define HAVE_STRUCT_TIMESPEC

#include <my_global.h>
#include <mysql.h>

#include "ConnectedSession.h"
#include "SessionManager.h"
#include "IOCPServer.h"
#include "DBManager.h"

#include "PacketProtocol.h"