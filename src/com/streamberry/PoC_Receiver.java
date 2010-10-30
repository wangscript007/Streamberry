package com.streamberry;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class PoC_Receiver implements Runnable {

	@Override
	public void run() {
		int port = 35489;

		System.out.println("Receiver thread started");
		listener(port);
	}

	private void listener(int port) {
		byte recvbuf[] = new byte[1024];
		// Arbitrarily chosen from unassigned multicast block
		// See:
		// http://www.iana.org/assignments/multicast-addresses/multicast-addresses.xml
		String multicastgroup = "224.0.0.133";

		try {
			MulticastSocket s = new MulticastSocket(port);

			s.joinGroup(InetAddress.getByName(multicastgroup));
			DatagramPacket p = new DatagramPacket(recvbuf, recvbuf.length);
			for (;;) {
				s.receive(p);
				System.out.println("Recieved data from " + p.getAddress().getCanonicalHostName());
			}
		} catch (IOException e) {
			System.err.println("Failed to make multicast socket (port " + port);
			e.printStackTrace();
			System.exit(1);
		}
	}

}
