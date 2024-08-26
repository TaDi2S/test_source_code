// test_pcap.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "pcap.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#pragma comment (lib, "wpcap.lib")
#pragma comment(lib, "Ws2_32.lib")

#define MIN_BYTE 2

u_char *fisrtData = nullptr;
int firstSize;
std::vector<std::pair<int, int>> sameList;
bool stopCapturing = false; // 패킷 캡처 중지 플래그

void compareFirstSeconf(const u_char* secondData, int secondSize) {

	for (int i = 0; i < firstSize - 1; i++) {
		for (int j = 0; j < secondSize - 1; j++) {
			if (fisrtData[i] == secondData[j] && fisrtData[i + 1] == secondData[j + 1]) {
				int start = i;
				int cnt = 2;
				while (1) {
					if (fisrtData[i + cnt] != secondData[j + cnt] || i + cnt == firstSize - 1 || j + cnt == secondSize - 1) {
						int end = i + cnt - 1;
						sameList.emplace_back(start, end);
						break;
					}
					cnt++;
				}
			}
		}
	}
}

void compareAnotherDatas(const u_char* udpData, int udpDataLen) {
	int listSize = sameList.size();
	std::vector<int> delIdx;
	for (int i = 0; i < listSize; i++) {
		bool flag = false;
		int sameLength = sameList[i].second - sameList[i].first + 1;
		for (int j = 0; j < udpDataLen - sameLength; j++) {
			if (std::strncmp((const char *)&fisrtData[sameList[i].first], (const char *)&udpData[j], sameLength) == 0) {
				flag = true;
			}
		}
		if (!flag) {
			delIdx.push_back(i);
		}
	}

	for (int i = delIdx.size() - 1; i >= 0; i--) {
		sameList.erase(sameList.begin() + delIdx[i]);
	}
}

// Ethernet 헤더 구조체
struct ethhdr {
	u_char  h_dest[6];    // destination ethernet address
	u_char  h_source[6];  // source ethernet address
	u_short h_proto;      // protocol type (IP, ARP, RARP, etc)
};

// IP 헤더 구조체
struct iphdr {
	u_char  ihl : 4, version : 4; // IP header length and version
	u_char  tos;              // Type of service
	u_short tot_len;          // Total length
	u_short id;               // Identification
	u_short frag_off;         // Fragment offset
	u_char  ttl;              // Time to live
	u_char  protocol;         // Protocol
	u_short check;            // Checksum
	u_long  saddr;            // Source address
	u_long  daddr;            // Destination address
};

// UDP 헤더 구조체
struct udphdr {
	u_short sport; // Source port
	u_short dport; // Destination port
	u_short len;   // Datagram length
	u_short crc;   // Checksum
};

// MAC 주소를 문자열로 변환하는 함수
std::string macToStr(const u_char *mac) {
	std::ostringstream oss;
	oss << std::hex << std::setfill('0');
	for (int i = 0; i < 6; ++i) {
		oss << std::setw(2) << static_cast<int>(mac[i]);
		if (i != 5) {
			oss << ":";
		}
	}
	return oss.str();
}

// IP 주소를 문자열로 변환하는 함수
std::string ipToStr(u_long ip) {
	struct in_addr ip_addr;
	ip_addr.s_addr = ip;
	return std::string(inet_ntoa(ip_addr));
}

u_int64 g_index = 1;

void printPacketInfo(const struct ethhdr *eth, const struct iphdr *ip, const struct udphdr *udp) {

	std::cout << "Ethernet Header" << std::endl;
	std::cout << "   |-Source Address      : " << macToStr(eth->h_source) << std::endl;
	std::cout << "   |-Destination Address : " << macToStr(eth->h_dest) << std::endl;
	//std::cout << "   |-Protocol            : " << ntohs(eth->h_proto) << std::endl;

	std::cout << "IP Header" << std::endl;
	std::cout << "   |-Version             : " << static_cast<int>(ip->version) << std::endl;
	std::cout << "   |-Header Length       : " << static_cast<int>(ip->ihl) << std::endl;
	std::cout << "   |-Type of Service     : " << static_cast<int>(ip->tos) << std::endl;
	std::cout << "   |-Total Length        : " << ntohs(ip->tot_len) << std::endl;
	std::cout << "   |-Identification      : " << ntohs(ip->id) << std::endl;
	std::cout << "   |-Time To Live        : " << static_cast<int>(ip->ttl) << std::endl;
	std::cout << "   |-Protocol            : " << static_cast<int>(ip->protocol) << std::endl;
	std::cout << "   |-Header Checksum     : " << ntohs(ip->check) << std::endl;
	std::cout << "   |-Source IP           : " << ipToStr(ip->saddr) << std::endl;
	std::cout << "   |-Destination IP      : " << ipToStr(ip->daddr) << std::endl;

	std::cout << "UDP Header" << std::endl;
	std::cout << "   |-Source Port         : " << ntohs(udp->sport) << std::endl;
	std::cout << "   |-Destination Port    : " << ntohs(udp->dport) << std::endl;
	std::cout << "   |-Length              : " << ntohs(udp->len) << std::endl;
	std::cout << "   |-Checksum            : " << ntohs(udp->crc) << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;
}
void printUDPDate(const u_char *udpData, int udpDataLen) {
	std::cout << "UDP Data" << std::endl;
	std::cout << "   |-Data Length         : " << udpDataLen << std::endl;
	std::cout << "   |-Data                : " << std::endl;
	for (int i = 0; i < udpDataLen; ++i) {
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(udpData[i]) << " ";
	}
	std::cout << std::dec << std::endl;
}

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {

	// 패킷 데이터를 여기서 처리할 수 있습니다.
	const struct ethhdr *eth = reinterpret_cast<const struct ethhdr*>(packet);
	if (ntohs(eth->h_proto) == 0x0800) { // IP 프로토콜
		const struct iphdr *ip = reinterpret_cast<const struct iphdr*>(packet + sizeof(struct ethhdr));
		if (ip->protocol == IPPROTO_UDP && pkthdr->caplen >= 400 && ipToStr(ip->saddr).compare("151.106.247.10") == 0) { // UDP 프로토콜
			const struct udphdr *udp = reinterpret_cast<const struct udphdr*>(packet + sizeof(struct ethhdr) + sizeof(struct iphdr));
			//std::cout << g_index << "번째 UDP 패킷" << std::endl;

			// UDP 데이터 추출
			int udpHeaderSize = sizeof(struct udphdr);
			int ipHeaderSize = ip->ihl * 4;
			int ethHeaderSize = sizeof(struct ethhdr);
			const u_char *udpData = packet + ethHeaderSize + ipHeaderSize + udpHeaderSize;
			int udpDataLen = ntohs(udp->len) - udpHeaderSize;
			if (udpDataLen >= 400) {


				//printPacketInfo(eth, ip, udp);

				
				//printUDPDate(udpData, udpDataLen);
				if (g_index == 1) {
				   fisrtData = new u_char[udpDataLen];
				   memcpy(fisrtData, udpData, udpDataLen);
				   firstSize = udpDataLen;
				}
				else if (g_index == 2) {
					compareFirstSeconf(udpData, udpDataLen);
				}
				else {
					compareAnotherDatas(udpData, udpDataLen);
				}
				
				g_index++;
			}
		}
	}

}

int main()
{
	char errbuf[PCAP_ERRBUF_SIZE];

	pcap_t *handle = pcap_open_offline("D:\\source\\test_source_code\\test_file\\pcap\\first.pcapng", errbuf);
	if (handle == nullptr) {
		std::cerr << "Could not open pcap file: " << errbuf << std::endl;
		return 1;
	}

	if (pcap_loop(handle, 0, packetHandler, nullptr) < 0) {
		std::cerr << "pcap_loop() failed: " << pcap_geterr(handle) << std::endl;
		return 1;
	}
end_loop:
	std::cout << "첫번째 두번째 겹치는 데이터들" << std::endl << "======================================" << std::endl;

	for (int i = 0; i < sameList.size(); ++i) {
		std::cout << sameList[i].first << "," << sameList[i].second << std::endl;
		for (int j = sameList[i].first; j <= sameList[i].second; j++) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(fisrtData[j]) << " ";
		}
		std::cout << std::dec << std::endl;
		std::cout << std::endl << "======================================" << std::endl;
	}
	
	pcap_close(handle);
	delete[] fisrtData;

}
