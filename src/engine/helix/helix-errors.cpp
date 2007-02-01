/***************************************************************************
 *   Copyright (C) 2005 Paul Cifarelli                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <q3dict.h>
#include <config.h>
#include <iostream>
#include "debug.h"
#include <klocale.h>
#include <map>

using namespace std;

#include "helix-errors.h"

struct HelixCoreErrors
{
   unsigned long code;
   QString error_string;
} helixErrors[500] =

{  {0x80040004, i18n("Invalid Operation")},
   {0x80040005, i18n("Invalid Version")},
   {0x80040006, i18n("Invalid Revision")},
   {0x80040007, i18n("Not Initialized")},
   {0x80040008, i18n("Doc Missing")},
   {0x80040009, i18n("Unexpected")},
   {0x8004000c, i18n("Incomplete")},
   {0x8004000d, i18n("Buffertoosmall")},
   {0x8004000e, i18n("Unsupported Video")},
   {0x8004000f, i18n("Unsupported Audio")},
   {0x80040010, i18n("Invalid Bandwidth")},
   {0x80040011, i18n("No Fileformat")},
   {0x80040011, i18n("No Fileformat")},
   {0x80040011, i18n("Missing Components")},
   {0x00040012, i18n("Element Not Found")},
   {0x00040013, i18n("Noclass")},
   {0x00040014, i18n("Class Noaggregation")},
   {0x80040015, i18n("Not Licensed")},
   {0x80040016, i18n("No Filesystem")},
   {0x80040017, i18n("Request Upgrade")},
   {0x80040018, i18n("Check Rights")},
   {0x80040019, i18n("Restore Server Denied")},
   {0x8004001a, i18n("Debugger Detected")},
   {0x8004005c, i18n("Restore Server Connect")},
   {0x8004005d, i18n("Restore Server Timeout")},
   {0x8004005e, i18n("Revoke Server Connect")},
   {0x8004005f, i18n("Revoke Server Timeout")},
   {0x800401cd, i18n("View Rights Nodrm")},
   {0x800401d3, i18n("Vsrc Nodrm")},
   {0x80040024, i18n("Wm Opl Not Supported")},
   {0x8004001b, i18n("Restoration Complete")},
   {0x8004001c, i18n("Backup Complete")},
   {0x8004001d, i18n("Tlc Not Certified")},
   {0x8004001e, i18n("Corrupted Backup File")},
   {0x8004001f, i18n("Awaiting License")},
   {0x80040020, i18n("Already Initialized")},
   {0x80040021, i18n("Not Supported")},
   {0x00040022, i18n("False")},
   {0x00040023, i18n("Warning")},
   {0x00040040, i18n("Buffering")},
   {0x00040041, i18n("Paused")},
   {0x00040042, i18n("No Data")},
   {0x00040043, i18n("Stream Done")},
   {0x80040043, i18n("Net Socket Invalid")},
   {0x80040044, i18n("Net Connect")},
   {0x80040045, i18n("Bind")},
   {0x80040046, i18n("Socket Create")},
   {0x80040047, i18n("Invalid Host")},
   {0x80040048, i18n("Net Read")},
   {0x80040049, i18n("Net Write")},
   {0x8004004a, i18n("Net Udp")},
   {0x8004004b, i18n("Retry")},
   {0x8004004c, i18n("Server Timeout")},
   {0x8004004d, i18n("Server Disconnected")},
   {0x8004004e, i18n("Would Block")},
   {0x8004004f, i18n("General Nonet")},
   {0x80040050, i18n("Block Canceled")},
   {0x80040051, i18n("Multicast Join")},
   {0x80040052, i18n("General Multicast")},
   {0x80040053, i18n("Multicast Udp")},
   {0x80040054, i18n("At Interrupt")},
   {0x80040055, i18n("Msg Toolarge")},
   {0x80040056, i18n("Net Tcp")},
   {0x80040057, i18n("Try Autoconfig")},
   {0x80040058, i18n("Notenough Bandwidth")},
   {0x80040059, i18n("Http Connect")},
   {0x8004005a, i18n("Port In Use")},
   {0x8004005b, i18n("Loadtest Not Supported")},
   {0x00040060, i18n("Tcp Connect")},
   {0x00040061, i18n("Tcp Reconnect")},
   {0x80040062, i18n("Tcp Failed")},
   {0x80040063, i18n("Authentication Socket Create Failure")},
   {0x80040064, i18n("Authentication Tcp Connect Failure")},
   {0x80040065, i18n("Authentication Tcp Connect Timeout")},
   {0x80040066, i18n("Authentication Failure")},
   {0x80040067, i18n("Authentication Required Parameter Missing")},
   {0x80040068, i18n("Dns Resolve Failure")},
   {0x00040068, i18n("Authentication Succeeded")},
   {0x80040069, i18n("Pull Authentication Failed")},
   {0x8004006a, i18n("Bind Error")},
   {0x8004006b, i18n("Pull Ping Timeout")},
   {0x8004006c, i18n("Authentication Tcp Failed")},
   {0x8004006d, i18n("Unexpected Stream End")},
   {0x8004006e, i18n("Authentication Read Timeout")},
   {0x8004006f, i18n("Authentication Connection Failure")},
   {0x80040070, i18n("Blocked")},
   {0x80040071, i18n("Notenough Predecbuf")},
   {0x80040072, i18n("End With Reason")},
   {0x80040073, i18n("Socket Nobufs")},
   {0x00040080, i18n("At End")},
   {0x80040081, i18n("Invalid File")},
   {0x80040082, i18n("Invalid Path")},
   {0x80040083, i18n("Record")},
   {0x80040084, i18n("Record Write")},
   {0x80040085, i18n("Temporary File")},
   {0x80040086, i18n("Already Open")},
   {0x80040087, i18n("Seek Pending")},
   {0x80040088, i18n("Cancelled")},
   {0x80040089, i18n("File Not Found")},
   {0x8004008a, i18n("Write Error")},
   {0x8004008b, i18n("File Exists")},
   {0x8004008c, i18n("File Not Open")},
   {0x0004008d, i18n("Advise Prefer Linear")},
   {0x8004008e, i18n("Parse Error")},
   {0x0004008f, i18n("Advise Noasync Seek")},
   {0x80040090, i18n("Header Parse Error")},
   {0x80040091, i18n("Corrupt File")},
   {0x800400c0, i18n("Bad Server")},
   {0x800400c1, i18n("Advanced Server")},
   {0x800400c2, i18n("Old Server")},
   {0x000400c3, i18n("Redirection")},
   {0x800400c4, i18n("Server Alert")},
   {0x800400c5, i18n("Proxy")},
   {0x800400c6, i18n("Proxy Response")},
   {0x800400c7, i18n("Advanced Proxy")},
   {0x800400c8, i18n("Old Proxy")},
   {0x800400c9, i18n("Invalid Protocol")},
   {0x800400ca, i18n("Invalid Url Option")},
   {0x800400cb, i18n("Invalid Url Host")},
   {0x800400cc, i18n("Invalid Url Path")},
   {0x800400cd, i18n("Http Content Not Found")},
   {0x800400ce, i18n("Not Authorized")},
   {0x800400cf, i18n("Unexpected Msg")},
   {0x800400d0, i18n("Bad Transport")},
   {0x800400d1, i18n("No Session Id")},
   {0x800400d2, i18n("Proxy Dnr")},
   {0x800400d3, i18n("Proxy Net Connect")},
   {0x800400d4, i18n("Aggregate Operation Not Allowed")},
   {0x800400d5, i18n("Rights Expired")},
   {0x800400d6, i18n("Not Modified")},
   {0x800400d7, i18n("Forbidden")},
   {0x80040100, i18n("Audio Driver Error")},
   {0x80040101, i18n("Late Packet")},
   {0x80040102, i18n("Overlapped Packet")},
   {0x80040103, i18n("Outoforder Packet")},
   {0x80040104, i18n("Noncontiguous Packet")},
   {0x80040140, i18n("Open Not Processed")},
   {0x80040141, i18n("Windraw Exception")},
   {0x80040180, i18n("Expired")},
   {0x80040fc0, i18n("Invalid Interleaver")},
   {0x80040fc1, i18n("Bad Format")},
   {0x80040fc2, i18n("Chunk Missing")},
   {0x80040fc3, i18n("Invalid Stream")},
   {0x80040fc4, i18n("Dnr")},
   {0x80040fc5, i18n("Open Driver")},
   {0x80040fc6, i18n("Upgrade")},
   {0x80040fc7, i18n("Notification")},
   {0x80040fc8, i18n("Not Notified")},
   {0x80040fc9, i18n("Stopped")},
   {0x80040fca, i18n("Closed")},
   {0x80040fcb, i18n("Invalid Wav File")},
   {0x80040fcc, i18n("No Seek")},
   {0x80040200, i18n("Decode Inited")},
   {0x80040201, i18n("Decode Not Found")},
   {0x80040202, i18n("Decode Invalid")},
   {0x80040203, i18n("Decode Type Mismatch")},
   {0x80040204, i18n("Decode Init Failed")},
   {0x80040205, i18n("Decode Not Inited")},
   {0x80040206, i18n("Decode Decompress")},
   {0x80040207, i18n("Obsolete Version")},
   {0x00040208, i18n("Decode At End")},
   {0x80040240, i18n("Encode File Too Small")},
   {0x80040241, i18n("Encode Unknown File")},
   {0x80040242, i18n("Encode Bad Channels")},
   {0x80040243, i18n("Encode Bad Sampsize")},
   {0x80040244, i18n("Encode Bad Samprate")},
   {0x80040245, i18n("Encode Invalid")},
   {0x80040246, i18n("Encode No Output File")},
   {0x80040247, i18n("Encode No Input File")},
   {0x80040248, i18n("Encode No Output Permissions")},
   {0x80040249, i18n("Encode Bad Filetype")},
   {0x8004024a, i18n("Encode Invalid Video")},
   {0x8004024b, i18n("Encode Invalid Audio")},
   {0x8004024c, i18n("Encode No Video Capture")},
   {0x8004024d, i18n("Encode Invalid Video Capture")},
   {0x8004024e, i18n("Encode No Audio Capture")},
   {0x8004024f, i18n("Encode Invalid Audio Capture")},
   {0x80040250, i18n("Encode Too Slow For Live")},
   {0x80040251, i18n("Encode Engine Not Initialized")},
   {0x80040252, i18n("Encode Codec Not Found")},
   {0x80040253, i18n("Encode Codec Not Initialized")},
   {0x80040254, i18n("Encode Invalid Input Dimensions")},
   {0x80040255, i18n("Encode Message Ignored")},
   {0x80040256, i18n("Encode No Settings")},
   {0x80040257, i18n("Encode No Output Types")},
   {0x80040258, i18n("Encode Improper State")},
   {0x80040259, i18n("Encode Invalid Server")},
   {0x8004025a, i18n("Encode Invalid Temp Path")},
   {0x8004025b, i18n("Encode Merge Fail")},
   {0x0004025c, i18n("Binary Data Not Found")},
   {0x0004025d, i18n("Binary End Of Data")},
   {0x8004025e, i18n("Binary Data Purged")},
   {0x8004025f, i18n("Binary Full")},
   {0x80040260, i18n("Binary Offset Past End")},
   {0x80040261, i18n("Encode No Encoded Data")},
   {0x80040262, i18n("Encode Invalid Dll")},
   {0x80040263, i18n("Not Indexable")},
   {0x80040264, i18n("Encode No Browser")},
   {0x80040265, i18n("Encode No File To Server")},
   {0x80040266, i18n("Encode Insufficient Disk Space")},
   {0x00040267, i18n("Encode Sample Discarded")},
   {0x80040268, i18n("Encode Rv10 Frame Too Large")},
   {0x00040269, i18n("Not Handled")},
   {0x0004026a, i18n("End Of Stream")},
   {0x0004026b, i18n("Jobfile Incomplete")},
   {0x0004026c, i18n("Nothing To Serialize")},
   {0x8004026d, i18n("Sizenotset")},
   {0x8004026e, i18n("Already Committed")},
   {0x8004026f, i18n("Buffers Outstanding")},
   {0x80040270, i18n("Not Committed")},
   {0x80040271, i18n("Sample Time Not Set")},
   {0x80040272, i18n("Timeout")},
   {0x80040273, i18n("Wrongstate")},
   {0x800403c1, i18n("Remote Usage Error")},
   {0x800403c2, i18n("Remote Invalid Endtime")},
   {0x800403c3, i18n("Remote Missing Input File")},
   {0x800403c4, i18n("Remote Missing Output File")},
   {0x800403c5, i18n("Remote Input Equals Output File")},
   {0x800403c6, i18n("Remote Unsupported Audio Version")},
   {0x800403c7, i18n("Remote Different Audio")},
   {0x800403c8, i18n("Remote Different Video")},
   {0x800403c9, i18n("Remote Paste Missing Stream")},
   {0x800403ca, i18n("Remote End Of Stream")},
   {0x800403cb, i18n("Remote Image Map Parse Error")},
   {0x800403cc, i18n("Remote Invalid Imagemap File")},
   {0x800403cd, i18n("Remote Event Parse Error")},
   {0x800403ce, i18n("Remote Invalid Event File")},
   {0x800403cf, i18n("Remote Invalid Output File")},
   {0x800403d0, i18n("Remote Invalid Duration")},
   {0x800403d1, i18n("Remote No Dump Files")},
   {0x800403d2, i18n("Remote No Event Dump File")},
   {0x800403d3, i18n("Remote No Imap Dump File")},
   {0x800403d4, i18n("Remote No Data")},
   {0x800403d5, i18n("Remote Empty Stream")},
   {0x800403d6, i18n("Remote Read Only File")},
   {0x800403d7, i18n("Remote Paste Missing Audio Stream")},
   {0x800403d8, i18n("Remote Paste Missing Video Stream")},
   {0x800403d9, i18n("Remote Encrypted Content")},
   {0x80040281, i18n("Property Not Found")},
   {0x80040282, i18n("Property Not Composite")},
   {0x80040283, i18n("Property Duplicate")},
   {0x80040284, i18n("Property Type Mismatch")},
   {0x80040285, i18n("Property Active")},
   {0x80040286, i18n("Property Inactive")},
   {0x80040287, i18n("Property Value Underflow")},
   {0x80040288, i18n("Property Value Overflow")},
   {0x80040289, i18n("Property Value less than Lower bound")},
   {0x8004028a, i18n("Property Value greater than Upper bound")},
   {0x0004028b, i18n("Property Delete Pending")},
   {0x800401c1, i18n("Could not initialize core")},
   {0x800401c2, i18n("Perfectplay Not Supported")},
   {0x800401c3, i18n("No Live Perfectplay")},
   {0x800401c4, i18n("Perfectplay Not Allowed")},
   {0x800401c5, i18n("No Codecs")},
   {0x800401c6, i18n("Slow Machine")},
   {0x800401c7, i18n("Force Perfectplay")},
   {0x800401c8, i18n("Invalid Http Proxy Host")},
   {0x800401c9, i18n("Invalid Metafile")},
   {0x800401ca, i18n("Browser Launch")},
   {0x800401cb, i18n("View Source Noclip")},
   {0x800401cc, i18n("View Source Dissabled")},
   {0x800401ce, i18n("Timeline Suspended")},
   {0x800401cf, i18n("Buffer Not Available")},
   {0x800401d0, i18n("Could Not Display")},
   {0x800401d1, i18n("Vsrc Disabled")},
   {0x800401d2, i18n("Vsrc Noclip")},
   {0x80040301, i18n("Resource Not Cached")},
   {0x80040302, i18n("Resource Not Found")},
   {0x80040303, i18n("Resource Close File First")},
   {0x80040304, i18n("Resource Nodata")},
   {0x80040305, i18n("Resource Badfile")},
   {0x80040306, i18n("Resource Partialcopy")},
   {0x800402c0, i18n("PayPerView No User")},
   {0x800402c1, i18n("PayPerView Guid Read Only")},
   {0x800402c2, i18n("PayPerView Guid Collision")},
   {0x800402c3, i18n("Register Guid Exists")},
   {0x800402c4, i18n("PayPerView Authorization Failed")},
   {0x800402c5, i18n("PayPerView Old Player")},
   {0x800402c6, i18n("PayPerView Account Locked")},
   {0x800402c8, i18n("Xr PayPerView Protocol Ignores")},
   {0x800402c9, i18n("PayPerView User Already Exists")},
   {0x80040340, i18n("Upg Auth Failed")},
   {0x80040341, i18n("Upg Cert Auth Failed")},
   {0x80040342, i18n("Upg Cert Expired")},
   {0x80040343, i18n("Upg Cert Revoked")},
   {0x80040344, i18n("Upg Rup Bad")},
   {0x80040345, i18n("Upg System Busy")},
   {0x80041800, i18n("Autocfg Success")},
   {0x80041901, i18n("No Error")},
   {0x80041902, i18n("Invalid Version")},
   {0x80041903, i18n("Invalid Format")},
   {0x80041904, i18n("Invalid Bandwidth")},
   {0x80041905, i18n("Invalid Path")},
   {0x80041906, i18n("Unknown Path")},
   {0x80041907, i18n("Invalid Protocol")},
   {0x80041908, i18n("Invalid Player Addr")},
   {0x80041909, i18n("Local Streams Prohibited")},
   {0x8004190a, i18n("Server Full")},
   {0x8004190b, i18n("Remote Streams Prohibited")},
   {0x8004190c, i18n("Event Streams Prohibited")},
   {0x8004190d, i18n("Invalid Host")},
   {0x8004190e, i18n("No Codec")},
   {0x8004190f, i18n("Livefile Invalid Bwn")},
   {0x80041910, i18n("Unable To Fulfill")},
   {0x80041911, i18n("Multicast Delivery Only")},
   {0x80041912, i18n("License Exceeded")},
   {0x80041913, i18n("License Unavailable")},
   {0x80041914, i18n("Invalid Loss Correction")},
   {0x80041915, i18n("Protocol Failure")},
   {0x80041916, i18n("Realvideo Streams Prohibited")},
   {0x80041917, i18n("Realaudio Streams Prohibited")},
   {0x80041918, i18n("Datatype Unsupported")},
   {0x80041919, i18n("Datatype Unlicensed")},
   {0x8004191a, i18n("Restricted Player")},
   {0x8004191b, i18n("Stream Initializing")},
   {0x8004191c, i18n("Invalid Player")},
   {0x8004191d, i18n("Player Plus Only")},
   {0x8004191e, i18n("No Embedded Players")},
   {0x8004191f, i18n("Pna Prohibited")},
   {0x80041920, i18n("Authentication Unsupported")},
   {0x80041921, i18n("Max Failed Authentications")},
   {0x80041922, i18n("Authentication Access Denied")},
   {0x80041923, i18n("Authentication Uuid Read Only")},
   {0x80041924, i18n("Authentication Uuid Not Unique")},
   {0x80041925, i18n("Authentication No Such User")},
   {0x80041926, i18n("Authentication Registration Succeeded")},
   {0x80041927, i18n("Authentication Registration Failed")},
   {0x80041928, i18n("Authentication Registration Guid Required")},
   {0x80041929, i18n("Authentication Unregistered Player")},
   {0x8004192a, i18n("Authentication Time Expired")},
   {0x8004192b, i18n("Authentication No Time Left")},
   {0x8004192c, i18n("Authentication Account Locked")},
   {0x8004192d, i18n("Authentication Invalid Server Cfg")},
   {0x8004192e, i18n("No Mobile Download")},
   {0x8004192f, i18n("No More Multi Addr")},
   {0x80041930, i18n("Proxy Max Connections")},
   {0x80041931, i18n("Proxy Max Gw Bandwidth")},
   {0x80041932, i18n("Proxy Max Bandwidth")},
   {0x80041933, i18n("Bad Loadtest Password")},
   {0x80041934, i18n("Pna Not Supported")},
   {0x80041935, i18n("Proxy Origin Disconnected")},
   {0x80041936, i18n("Internal Error")},
   {0x80041937, i18n("Max Value")},
   {0x80040600, i18n("Socket Intr")},
   {0x80040601, i18n("Socket Badf")},
   {0x80040602, i18n("Socket Acces")},
   {0x80040603, i18n("Socket Fault")},
   {0x80040604, i18n("Socket Inval")},
   {0x80040605, i18n("Socket Mfile")},
   {0x80040606, i18n("Socket Wouldblock")},
   {0x80040607, i18n("Socket Inprogress")},
   {0x80040608, i18n("Socket Already")},
   {0x80040609, i18n("Socket Notsock")},
   {0x8004060a, i18n("Socket Destaddrreq")},
   {0x8004060b, i18n("Socket Msgsize")},
   {0x8004060c, i18n("Socket Prototype")},
   {0x8004060d, i18n("Socket Noprotoopt")},
   {0x8004060e, i18n("Socket Protonosupport")},
   {0x8004060f, i18n("Socket Socktnosupport")},
   {0x80040610, i18n("Socket Opnotsupp")},
   {0x80040611, i18n("Socket Pfnosupport")},
   {0x80040612, i18n("Socket Afnosupport")},
   {0x80040613, i18n("Socket Addrinuse")},
   {0x80040614, i18n("Socket Address Not Available")},
   {0x80040615, i18n("Socket Net Down")},
   {0x80040616, i18n("Socket Net Unreachable")},
   {0x80040617, i18n("Socket Net Reset")},
   {0x80040618, i18n("Socket Connection Aborted")},
   {0x80040619, i18n("Socket Connection Reset")},
   {0x8004061a, i18n("Socket No buffers")},
   {0x8004061b, i18n("Socket Isconnected")},
   {0x8004061c, i18n("Socket Notconn")},
   {0x8004061d, i18n("Socket Shutdown")},
   {0x8004061e, i18n("Socket Too Many References")},
   {0x8004061f, i18n("Socket Timedout")},
   {0x80040620, i18n("Socket Connection Refused")},
   {0x80040621, i18n("Socket Loop")},
   {0x80040622, i18n("Socket Name too long")},
   {0x80040623, i18n("Socket Hostdown")},
   {0x80040624, i18n("Socket Hostunreach")},
   {0x80040625, i18n("Socket Pipe")},
   {0x80040626, i18n("Socket Endstream")},
   {0x00040627, i18n("Socket Buffered")},
   {0x80040640, i18n("Resolve Noname")},
   {0x80040641, i18n("Resolve Nodata")},
   {0, 0}
   };


class HelixErrorsBase
{
public:
   HelixErrorsBase();
   ~HelixErrorsBase();

   QString *errorText(unsigned long code);

private:
   std::map<unsigned long, QString *> m_errors;
   int                                m_nerrors;
};


HelixErrorsBase *HelixErrors::m_base     = new HelixErrorsBase();

QString *HelixErrors::errorText(unsigned long code)
{
   return m_base->errorText(code);
}

HelixErrorsBase::HelixErrorsBase() : m_nerrors(0)
{
   while (helixErrors[m_nerrors].code) m_nerrors++;

   for (int i=0; i<m_nerrors; i++)
      m_errors[helixErrors[i].code] = new QString(helixErrors[i].error_string);
}

HelixErrorsBase::~HelixErrorsBase()
{
   for (int i=0; i<m_nerrors; i++)
      delete m_errors[helixErrors[i].code];
}

QString *HelixErrorsBase::errorText(unsigned long code)
{
   if (m_errors.count(code))
      return m_errors[code];
   else
      return 0;
}
