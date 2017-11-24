#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <iostream>
#include "ns3/csma-module.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("3rd Lab Program");
//MyApp class inherits the ns-3 Application class defined in
//src/network/model/application.h.
//The MyApp class is obligated to override the StartApplication and
//StopApplication methods. These methods are automatically called when MyApp is
//required to start and stop sending data during the simulation.

class MyApp : public Application
{
public:
    MyApp ();
    virtual ~MyApp();
    void Setup (Ptr<Socket> socket, Address address, uint32_t packetSize, uint32_t
    nPackets, DataRate dataRate);
private:
    virtual void StartApplication (void);
    virtual void StopApplication (void);
    void ScheduleTx (void);
    void SendPacket (void);
    Ptr<Socket>
    m_socket;
    Address
    m_peer;
    uint32_t
    m_packetSize;
    uint32_t
    m_nPackets;
    DataRate
    m_dataRate;
    EventId
    m_sendEvent;
    bool
    m_running;
    uint32_t
    m_packetsSent;
};
MyApp::MyApp ()
// constructor
: m_socket (0),
m_peer (),
m_packetSize (0),
m_nPackets (0),
m_dataRate (0),
m_sendEvent (),
m_running (false),
m_packetsSent (0)
{
}
MyApp::~MyApp()
// destructor
{
    m_socket = 0;
}
// initialize member variables.
void MyApp::Setup (Ptr<Socket> socket, Address address, uint32_t packetSize,
                   uint32_t nPackets, DataRate dataRate)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
}
// Below code is the overridden implementation of Application::StartApplication. It
//does a socket bind operation and establishes TCP connection with the address
//specified in m_peer.
void MyApp::StartApplication (void)
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind ();
    m_socket->Connect (m_peer);
    SendPacket ();
}
//The next bit of code explains to the Application how to stop creating simulation
//events.
void MyApp::StopApplication (void)
{
    m_running = false;
    if (m_sendEvent.IsRunning ())
    {
        Simulator::Cancel (m_sendEvent);
    }
    if (m_socket)
    {
        m_socket->Close ();
    }
}
//StartApplication calls SendPacket to start the chain of events that describes the
//Application behavior.
void MyApp::SendPacket (void)
{
    Ptr<Packet> packet = Create<Packet> (m_packetSize);
    m_socket->Send (packet);
    if (++m_packetsSent < m_nPackets)
    {
        ScheduleTx ();
    }
}
//It is the responsibility of the Application to keep scheduling the chain of
//events, so the next lines call ScheduleTx to schedule another transmit event
//(a SendPacket) until the Application decides it has sent enough.
void MyApp::ScheduleTx (void)
{
    if (m_running)
    {
        Time tNext (Seconds (m_packetSize * 8 / static_cast<double> (m_dataRate.GetBitRate
        ())));
        m_sendEvent = Simulator::Schedule (tNext, &MyApp::SendPacket, this);
    }
}
//Below function logs the current simulation time and the new value of the
congestion window every time it is changed.
static void CwndChange (uint32_t oldCwnd, uint32_t newCwnd)
{
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () <<"\t"<< newCwnd);
}
//trace sink to show where packets are dropped
static void RxDrop (Ptr<const Packet> p)
{
    NS_LOG_UNCOND ("RxDrop at "<< Simulator::Now ().GetSeconds ());
}
//main function
int main (int argc, char *argv[])
{
    CommandLine cmd;
    cmd.Parse (argc, argv);
    NS_LOG_INFO ("Create nodes.");
    NodeContainer nodes;
    nodes.Create (4); //4 csma nodes are created
    CsmaHelper csma;
    csma.SetChannelAttribute ("DataRate", StringValue ("5Mbps"));
    csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (0.0001)));
    NetDeviceContainer devices;
    devices = csma.Install (nodes);
    //RateErrorModel allows us to introduce errors into a Channel at a given rate.
    Ptr<RateErrorModel>em = CreateObject<RateErrorModel> ();
    em->SetAttribute ("ErrorRate", DoubleValue (0.00001));
    devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
    InternetStackHelper stack;
    stack.Install (nodes);
    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign (devices);
    uint16_t sinkPort = 8080;
    //PacketSink Application is used on the destination node to receive TCP connections
    //and data.
    Address sinkAddress (InetSocketAddress (interfaces.GetAddress (1), sinkPort));
    PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress
    (Ipv4Address::GetAny (), sinkPort));
    ApplicationContainer sinkApps = packetSinkHelper.Install (nodes.Get (1));
    sinkApps.Start (Seconds (0.));
    sinkApps.Stop (Seconds (20.));
    //next two lines of code will create the socket and connect the trace source.
    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (nodes.Get (0),
                                                     TcpSocketFactory::GetTypeId ());
    ns3TcpSocket->TraceConnectWithoutContext ("CongestionWindow", MakeCallback
    (&CwndChange));
    //creates an Object of type MyApp
    Ptr<MyApp> app = CreateObject<MyApp> ();
    //tell the Application what Socket to use, what address to connect to, how much
    //data to send at each send event, how many send events to generate and the rate at
    //which to produce data from those events.
    app->Setup (ns3TcpSocket, sinkAddress, 1040, 1000, DataRate ("50Mbps"));
    nodes.Get (0)->AddApplication (app);
    app->SetStartTime (Seconds (1.));
    app->SetStopTime (Seconds (20.));
    devices.Get (1)->TraceConnectWithoutContext ("PhyRxDrop", MakeCallback (&RxDrop));
    Simulator::Stop (Seconds (20));
    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
}

