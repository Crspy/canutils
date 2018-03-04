#ifndef SRC_LINUX_SOCKETCANINFOLINUX_H_
#define SRC_LINUX_SOCKETCANINFOLINUX_H_

#include "SocketCanInfoImpl.h"
#include "netlink/Netlink.h"
#include "netlink/NetlinkCanParser.h"

namespace CanUtils
{

class SocketCanInfoLinux final :  public SocketCanInfoImpl
{
public:
	SocketCanInfoLinux() = delete;
	explicit SocketCanInfoLinux(const std::string& device_arg);
	virtual ~SocketCanInfoLinux();
	
protected:
	virtual int  openDevice() override final;
	virtual int  closeDevice() override final;
	virtual bool deviceIsOpen() override final;
	virtual CANDeviceInfoPtr  readDevice() override final;
	
private:
	static int copyDetails   (CANDeviceInfoPtr info, Netlink::NetlinkParser::DeviceDetailsPtr dev);
	static int copyCanDetails(CANDeviceInfoPtr info, Netlink::NetlinkCanParser::CanDeviceDetailsPtr cdev);
	
	static std::vector<DeviceFlags> copyDeviceFlags(std::vector<Netlink::NetlinkParser::DeviceFlags>&    vec);
	static std::vector<ControlMode> copyControlMode(std::vector<Netlink::NetlinkCanParser::ControlMode>& vec);
	
protected:
	std::string device;
	
	Netlink::NetlinkPtr netlink;
};

} /* namespace CanUtils */

#endif // SRC_LINUX_SOCKETCANINFOLINUX_H_
