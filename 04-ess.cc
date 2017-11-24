#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

int main (int argc, char *argv[])
{
    bool verbose = true;
    uint32_t nWifi = 3; // 3 wi-fi nodes are created
    CommandLine cmd;
    cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
    cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
    cmd.Parse (argc,argv);
    if (verbose)
    {
        LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }
    NodeContainer p2pNodes;
    p2pNodes.Create (2); // 2 nodes are n0,n1 are created
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install (p2pNodes);
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create (nWifi);
    NodeContainer wifiApNode = p2pNodes.Get (0);// 1st node of p2p is also access point
    // default PHY layer configuration is used for wifi
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
    phy.SetChannel (channel.Create ());
    WifiHelper wifi;
    wifi.SetRemoteStationManager ("ns3::AarfWifiManager");//AARF= rate control algorithm
    WifiMacHelper mac;
    Ssid ssid = Ssid ("ns-3-ssid");// ssid=service set identifier in 802.11
    mac.SetType ("ns3::StaWifiMac",
                 "Ssid", SsidValue (ssid),
                 "ActiveProbing", BooleanValue (false));
    NetDeviceContainer staDevices;
    staDevices = wifi.Install (phy, mac, wifiStaNodes);
    mac.SetType ("ns3::ApWifiMac","Ssid", SsidValue (ssid));
    NetDeviceContainer apDevices;
    apDevices = wifi.Install (phy, mac, wifiApNode);
    MobilityHelper mobility;
    // 2 dimensional grid to initially place sta(stationary nodes)
    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (10.0),
                                   "MinY", DoubleValue (-10.0),
                                   "DeltaX", DoubleValue (7.0),
                                   "DeltaY", DoubleValue (12.0),
                                   "GridWidth", UintegerValue (3),
                                   "LayoutType", StringValue ("RowFirst"));
    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                               "Bounds",RectangleValue (Rectangle (-50, 50, -50, 50)));
    mobility.Install (wifiStaNodes);
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (wifiApNode);
    InternetStackHelper stack;
    stack.Install (p2pNodes.Get(1));// stack installed on n1 of p2p
    stack.Install (wifiApNode); //stack installed on access point
    stack.Install (wifiStaNodes); //stack installed on mobile nodes
    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign (p2pDevices);
    address.SetBase ("10.1.3.0", "255.255.255.0");
    address.Assign (staDevices);
    address.Assign (apDevices);
    //install echo server application on n1
    UdpEchoServerHelper echoServer (9);
    ApplicationContainer serverApps = echoServer.Install (p2pNodes.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));
    //install echo client application on n3
    UdpEchoClientHelper echoClient (p2pInterfaces.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
    ApplicationContainer clientApps =
    echoClient.Install (wifiStaNodes.Get (nWifi - 1));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (10.0));
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    Simulator::Stop (Seconds (10.0));
    AsciiTraceHelper ascii;
    pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("Tracefilewifides.tr"));
    phy.EnableAsciiAll (ascii.CreateFileStream ("Tracefilewifisrc.tr"));
    Simulator::Run ();
    Simulator::Destroy();
    return 0;
}

