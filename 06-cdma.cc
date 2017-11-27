#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/config-store.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("CDMA Program");

int main(int argc, char *argv[]) {
    uint16_t numberOfNodes = 5;
    double simTime = 1.1;
    double distance = 60.0;
    double interPacketInterval = 100;
    CommandLine cmd;
    cmd.Parse(argc, argv);
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
    lteHelper->SetEpcHelper(epcHelper);
    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();
    cmd.Parse(argc, argv);

    Ptr<Node> pgw = epcHelper->GetPgwNode();

    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);
    InternetStackHelper internet;
    internet.Install(remoteHostContainer);

    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(1500));
    p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.010)));
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress(1);

    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);

    NodeContainer ueNodes;
    NodeContainer enbNodes;
    enbNodes.Create(numberOfNodes);
    ueNodes.Create(numberOfNodes);

    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();

    for(uint16_t i = 0; i < numberOfNodes; i++)
        positionAlloc->Add(Vector(distance * i, 100, 100));

    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.SetPositionAllocator(positionAlloc);
    mobility.Install(enbNodes);
    mobility.Install(ueNodes);

    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice(ueNodes);

    internet.Install(ueNodes);
    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs));

    for(uint32_t u = 0; u < ueNodes.GetN(); ++u) {
        Ptr<Node> ueNode = ueNodes.Get(u);
        Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting(ueNode->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    for(uint16_t i = 0; i < numberOfNodes; i++) {
        lteHelper->Attach(ueLteDevs.Get(i), enbLteDevs.Get(i));
    }

    uint16_t dlPort = 1234;
    uint16_t ulPort = 2000;
    uint16_t otherPort = 3000;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;

    for(uint32_t u = 0; u < ueNodes.GetN(); ++u) {
        ++ulPort;
        ++otherPort;
        PacketSinkHelper dlPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), dlPort));
        PacketSinkHelper ulPacketSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), ulPort));
        PacketSinkHelper packetSinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), otherPort));
        serverApps.Add(dlPacketSinkHelper.Install(ueNodes.Get(u)));
        serverApps.Add(ulPacketSinkHelper.Install(remoteHost));
        serverApps.Add(packetSinkHelper.Install(ueNodes.Get(u)));
        
        UdpClientHelper dlClient(ueIpIface.GetAddress(u), dlPort);
        dlClient.SetAttribute("Interval", TimeValue(MilliSeconds(interPacketInterval)));
        dlClient.SetAttribute("MaxPackets", UintegerValue(1000000));

        UdpClientHelper ulClient(remoteHostAddr, ulPort);
        ulClient.SetAttribute("Interval", TimeValue(MilliSeconds(interPacketInterval)));
        ulClient.SetAttribute("MaxPackets", UintegerValue(1000000));

        UdpClientHelper client(ueIpIface.GetAddress(u), otherPort);
        client.SetAttribute("Interval", TimeValue(MilliSeconds(interPacketInterval)));
        client.SetAttribute("MaxPackets", UintegerValue(1000000));

        clientApps.Add(dlClient.Install(remoteHost));
        clientApps.Add(ulClient.Install(ueNodes.Get(u)));
        if(u + 1 < ueNodes.GetN()) {
            clientApps.Add(client.Install(ueNodes.Get(u+1)));
        } else {
            clientApps.Add(client.Install(ueNodes.Get(0)));
        }
    }

    serverApps.Start(Seconds(0.01));
    clientApps.Start(Seconds(0.01));

    lteHelper->EnableTraces();
    AsciiTraceHelper ascii;
    p2ph.EnableAsciiAll(ascii.CreateFileStream("cdma.tr"));

    Simulator::Stop(Seconds(simTime));
    Simulator::Run();

    Simulator::Destroy();
    return 0;
}
