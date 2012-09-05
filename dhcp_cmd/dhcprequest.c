#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>

#include <isc-dhcp/result.h>
#include <dhcpctl.h>

#define DEFAULT_SERVER "localhost"
#define DEFAULT_PORT   10004

void usage();
void str2mac(unsigned char *mac,const char *str);

static int _init(const char *,const int);
static int _create();
static int _remove();

dhcpctl_handle conn = NULL;
dhcpctl_handle host = NULL;

dhcpctl_data_string ipaddr  = NULL;
dhcpctl_data_string macaddr = NULL;

char *str_host = NULL;
char *str_ip   = NULL;
char *str_mac  = NULL;

char *cmd;

int 
main (int argc, char **argv)
{
    struct in_addr convaddr;
    unsigned char  mac[6];
    int ret = 0;
    
    char *server = DEFAULT_SERVER;
    int  port    = DEFAULT_PORT;



    while((ret = getopt(argc,argv,"s:p:?h")) > 0) {
	switch(ret){
	case 's':
	    server = optarg;
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
	case 'h':
	case '?':
	    usage();
	    return(EXIT_SUCCESS);
	}
    }

    cmd      = argv[optind];
    if( !strcmp(cmd,"create") && argc > optind + 3){
	str_host = argv[optind+1];
	str_ip   = argv[optind+2];
	str_mac  = argv[optind+3];
	
	ret = _init(server,port);
	if(ret < 0){
	    fprintf(stderr,"%s:%d Can not Connect!\n",server,port);
	}	    
	str2mac(mac,str_mac);
	
	inet_pton(AF_INET, str_ip ,&convaddr);

	memcpy(ipaddr->value,  &convaddr.s_addr, 4);
	memcpy(macaddr->value, &mac,  6);

	ret = _create();
    } else if(!strcmp(cmd,"remove") && argc > optind + 1){
	str_host = argv[optind+1];
	_init(server,port);
	ret = _remove();
    } else {
	usage();
	return(EXIT_FAILURE);
    }
    
    if(ret != 0){
	return(EXIT_FAILURE);
    } else {
	return(EXIT_SUCCESS);
    }
}


void
usage(){
    printf("Usage\n"); 
    printf("  ./dhcprequest [-p port] [-s server]create host ip mac\n");
    printf("  ./dhcprequest [-p port] [-s server]remove host \n");
    printf("  default server %s\n",DEFAULT_SERVER);
    printf("  default port   %d\n",DEFAULT_PORT);
}


static int
_init(const char *server , const int port)
{
  /* DHCP Control */
  dhcpctl_initialize ();
  
  dhcpctl_connect (&conn,server,port, 0);
  if(!conn){
    return(-1);
  }

  omapi_data_string_new (&ipaddr  , 4, MDL);
  memset (ipaddr,  0, sizeof(ipaddr));

  omapi_data_string_new (&macaddr , 6, MDL);
  memset (macaddr, 0, sizeof(macaddr));

  return(0);
}

static int
_create()
{
  isc_result_t waitstatus;
  isc_result_t status;
  
  status = dhcpctl_new_object (&host, conn,"host");
  status = dhcpctl_set_string_value  (host, str_host, "name");
  status = dhcpctl_set_boolean_value (host, 1,        "hardware-type");
  status = dhcpctl_set_value         (host, ipaddr,   "ip-address");
  status = dhcpctl_set_value         (host, macaddr,  "hardware-address");

  status = dhcpctl_open_object (host, conn, DHCPCTL_CREATE | DHCPCTL_EXCL);
  
  status = dhcpctl_wait_for_completion (host,&waitstatus);
  if (waitstatus != ISC_R_SUCCESS) {
    return(-1);
  }
  
  return(0);
}

static int 
_remove()
{
  isc_result_t waitstatus;
  isc_result_t status;
  
  status = dhcpctl_new_object (&host, conn,"host");
  status = dhcpctl_set_string_value  (host, str_host, "name");
  status = dhcpctl_open_object (host, conn, 0);
  
  status = dhcpctl_wait_for_completion (host,&waitstatus);
  if (waitstatus != ISC_R_SUCCESS) {
    return(-1);
 }  
  
  status = dhcpctl_object_remove(conn,host);
  status = dhcpctl_wait_for_completion (host,&waitstatus);
  if (waitstatus != ISC_R_SUCCESS) {
    return(-1);
  }
  return(0);
}

void
str2mac(unsigned char *mac,const char *str)
{
  int i,j;
  for(i = 0; i < 6; i++){
      mac[i] = 0;
  }
  i = 0;
  for(j = 0 ; j < strlen(str); j++){
    if(str[j] == ':'){
      i++;
    } else if(isxdigit(str[j])){
      mac[i] *= 0x10;
      if(isupper(str[j])){
      mac[i] += str[j] - 'A' + 10 ;
      } else if (islower(str[j])){
      mac[i] += str[j] - 'a' + 10;
      } else {
	mac[i] += str[j] - '0';
      }
    }
  }
}
