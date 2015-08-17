/*
 * http://myself659.github.io/
 * 
 */


/* ͷ�ļ� */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


/* ����ģ��handler  */
static char * ngx_http_hello(ngx_conf_t * cf, ngx_command_t * cmd, void * conf);

/* ����http fun ��Ӧhttp���� body */
static u_char ngx_hello_string[] = "hello nginx!\r\n";


/* ����helloģ��ָ� */
static ngx_command_t ngx_http_hello_commands[] = 
{
	{ 
		// ģ����Ϊhello 
		ngx_string("hello"),
		// 
		NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
		// A pointer to our handler-function.
		ngx_http_hello,
		// We're not using these two, they're related to the configuration structures.
		0, 0,
		// A pointer to a post-processing handler. We're not using any here.
		NULL 
	},
	
	ngx_null_command // �����Կ�ָ����Ϊ��ָ��Ľ��� 
};

/* ����ģ��������  helloģ�鲻�漰 */
static ngx_http_module_t ngx_http_hello_module_ctx = 
{
	NULL, // preconfiguration
	NULL, // postconfiguration
	NULL, // create main configuration
	NULL, // init main configuration
	NULL, // create server configuration
	NULL, // merge server configuration
	NULL, // create location configuration
	NULL // merge location configuration
};


/* ����ģ�� */
ngx_module_t ngx_http_hello_module = {
	NGX_MODULE_V1,
	&ngx_http_hello_module_ctx, // ģ��������
	ngx_http_hello_commands, // ģ������
	NGX_HTTP_MODULE, // ģ������ 
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	NGX_MODULE_V1_PADDING
};


static ngx_int_t
ngx_http_hello_handler(ngx_http_request_t * r)
{
	ngx_int_t rc;
	ngx_buf_t * b;
	ngx_chain_t out;
	
	// ֻ����NGX_HTTP_GET �� NGX_HTTP_HEAD ���� 
	if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) 
	{
		return NGX_HTTP_NOT_ALLOWED;
	}
	
	// ����Content-type
	r->headers_out.content_type_len = sizeof("text/html") - 1;
	r->headers_out.content_type.len = sizeof("text/html") - 1;
	r->headers_out.content_type.data = (u_char * ) "text/html";

	// �����������ΪNGX_HTTP_HEAD��ֻ����httpͷ
	if (r->method == NGX_HTTP_HEAD) 
	{
		r->headers_out.status = NGX_HTTP_OK;
		r->headers_out.content_length_n = sizeof(ngx_hello_string) - 1;
		return ngx_http_send_header(r);
	}
	// ���ڴ������һ��ngx_buf 
	b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
	if (b == NULL) 
	{
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	

	// ����ngx buf
	b->pos = ngx_hello_string;
	b->last = ngx_hello_string + sizeof(ngx_hello_string) - 1;
	b->memory = 1; // ngx buf��ֹ�޸�
	b->last_buf = 1; // ��ʾ��ngx buf Ϊbuf chain���һ�黺��
	
	// ��ngx buf ���� buf chain 
	out.buf = b;
	out.next = NULL;
	
	// ����״̬�� 
	r->headers_out.status = NGX_HTTP_OK;
	// ����content����
	r->headers_out.content_length_n = sizeof(ngx_hello_string) - 1;

	// ���ͻ�Ӧhttpͷ
	rc = ngx_http_send_header(r);
	if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) 
	{
		return rc;
	}
	
	// ���ͻ�Ӧbody 
	return ngx_http_output_filter(r, &out);
	
}


/* ����commmand ��Ӧ�Ĵ���handler */
static char *
ngx_http_hello(ngx_conf_t * cf, ngx_command_t * cmd, void * conf)
{
	ngx_http_core_loc_conf_t * clcf;
	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_hello_handler; 
	return NGX_CONF_OK;
}





