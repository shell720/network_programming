#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int open_connection(char *host, char *service); //サーバーとのTCP接続、socket()の返り値のファイルディスクリプションをreturn

int main (int argc, char *argv[]){
    int sock;
    FILE *f;
    char buf[1024];
    char *hostName = argv[1];

    if (argc<2) hostName = "localhost";
    sock = open_connection(hostName, "daytime");
    f = fdopen(sock, "r");
    //上の行で失敗したらここでエラー処理する
    fgets(buf, sizeof buf, f);
    fclose(f);
    puts(buf);
}

static int open_connection(char *host, char *service){
    int sock, err;
    struct addrinfo hints, *res, *ai;

    memset(&hints, 0 ,sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;
    err = getaddrinfo(host, service, &hints, &res); printf("%d", err);
    //本来は上の行でerrが出たら例外処理をする
    for (ai=res; ; ai= ai->ai_next){
        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sock<0) continue;
        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            continue;
        }

        //接続成功
        freeaddrinfo(res);
        return sock;
    }

    //接続に失敗
    freeaddrinfo(res);
}