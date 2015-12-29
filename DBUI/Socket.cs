using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Diagnostics;
using DBUI;

// State object for receiving data from remote device.
public class StateObject {
    // Client socket.
    public Socket workSocket = null;
    // Size of receive buffer.
    public const int BufferSize = 256;
    // Receive buffer.
    public byte[] buffer = new byte[BufferSize];
    // Received data string.
    public StringBuilder sb = new StringBuilder();
}

public class AsynchronousClient
{
    // The port number for the remote device.
    public static int port = 11000;
    public static IPAddress ipAddress;

    // ManualResetEvent instances signal completion.
    public static ManualResetEvent connectDone =
        new ManualResetEvent(false);
    public static ManualResetEvent sendDone =
        new ManualResetEvent(false);
    public static ManualResetEvent receiveDone =
        new ManualResetEvent(false);

    // The response from the remote device.
    private static String response = String.Empty;

    public static String get_response()
    {
        return response;
    }

    public static void StartClient()
    {
        // Connect to a remote device.
        try
        {
            // Establish the remote endpoint for the socket.
            // The name of the 
            // remote device is "host.contoso.com".
            //IPHostEntry ipHostInfo = Dns.Resolve("host.contoso.com");
            
            IPEndPoint remoteEP = new IPEndPoint(ipAddress, port);

            // Create a TCP/IP socket.
            client = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);

            // Connect to the remote endpoint.
            client.BeginConnect(remoteEP,
                new AsyncCallback(ConnectCallback), client);
            connectDone.WaitOne();

            // Send test data to the remote device.
            //Send("This is a test<EOF>");
            //sendDone.WaitOne();

            // Receive the response from the remote device.
            //Receive(client);
            //receiveDone.WaitOne();

            // Write the response to the console.
            //Console.WriteLine("Response received : {0}", response);


        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }

    public static void CloseSocket()
    {
        // Release the socket.
        client.Shutdown(SocketShutdown.Both);
        client.Close();
    }

    private static void ConnectCallback(IAsyncResult ar)
    {
        try
        {
            // Retrieve the socket from the state object.
            Socket client = (Socket)ar.AsyncState;

            // Complete the connection.
            client.EndConnect(ar);

            Console.WriteLine("Socket connected to {0}",
                client.RemoteEndPoint.ToString());

            // Signal that the connection has been made.
            connectDone.Set();
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
            connectDone.Set();
        }
    }

    public static void Receive0()
    {
        try
        {
            // Create the state object.
            StateObject state = new StateObject();
            state.workSocket = client;

            // Begin receiving the data from the remote device.
            //client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
            //    new AsyncCallback(ReceiveCallback), state);
            int bytesRead;
            response = "";
            while (true)
            {
                bytesRead = client.Receive(state.buffer);
                state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                Debug.WriteLine("Received: " + state.sb.ToString() + "\n");
                if (state.sb.ToString().Length >= 4 && state.sb.ToString().Substring(state.sb.ToString().Length - 4) == "#end")
                {
                    response = state.sb.ToString();
                    //form.setData(response);
                    return;
                }
            }

        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }

    public static void Receive()
    {
        try
        {
            // Create the state object.
            StateObject state = new StateObject();
            state.workSocket = client;
            Debug.WriteLine(state.sb.ToString() + "\n");
            // Begin receiving the data from the remote device.
            client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
                new AsyncCallback(ReceiveCallback), state);
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }

    private static void ReceiveCallback(IAsyncResult ar)
    {
        try
        {
            // Retrieve the state object and the client socket 
            // from the asynchronous state object.
            StateObject state = (StateObject)ar.AsyncState;
            Socket client = state.workSocket;

            // Read data from the remote device.
            int bytesRead = client.EndReceive(ar);

            if (bytesRead > 0)
            {
                
                // There might be more data, so store the data received so far.
                state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                Debug.WriteLine(state.sb.ToString());
                if (state.sb.ToString().Length >= 4 && state.sb.ToString().Substring(state.sb.ToString().Length - 4) == "#end")
                {
                    response = state.sb.ToString();
                    //form.setData(response.Substring(0, response.Length - 4));
                    receiveDone.Set();
                    return;
                }

                // Get the rest of the data.
                client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0,
                    new AsyncCallback(ReceiveCallback), state);
            }
            else
            {
                // All the data has arrived; put it in response.
                if (state.sb.Length > 1)
                {
                    response = state.sb.ToString();
                    //form.setData(response);
                }
                // Signal that all bytes have been received.
                receiveDone.Set();
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }

    public static void Send(String data)
    {
        // Convert the string data to byte data using ASCII encoding.
        byte[] byteData = Encoding.ASCII.GetBytes(data);
        Array.Resize(ref byteData, data.Length + 1);
        byteData[data.Length] = 0;

        // Begin sending the data to the remote device.
        client.BeginSend(byteData, 0, byteData.Length, 0,
            new AsyncCallback(SendCallback), client);
    }

    public static void SendCallback(IAsyncResult ar)
    {
        try
        {
            // Retrieve the socket from the state object.
            Socket client = (Socket)ar.AsyncState;

            // Complete sending the data to the remote device.
            int bytesSent = client.EndSend(ar);
            Console.WriteLine("Sent {0} bytes to server.", bytesSent);

            // Signal that all bytes have been sent.
            sendDone.Set();
        }
        catch (Exception e)
        {
            Console.WriteLine(e.ToString());
        }
    }

    public static Form1 form;

    private static Socket client;
}
