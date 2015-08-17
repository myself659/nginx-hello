/*
 * http://myself659.github.io/
 * 
 */


/* 头文件 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


/* 声明模块handler  */
static char * ngx_http_hello(ngx_conf_t * cf, ngx_command_t * cmd, void * conf);

/* 定义http fun 回应http报文 body */
static u_char ngx_hello_string[] = "hello nginx!\r\n";


/* 定义hello模块指令集 */
static ngx_command_t ngx_http_hello_commands[] = 
{
	{ 
		// 模块名为hello 
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
	
	ngx_null_command // 必须以空指令作为该指令集的结束 
};

/* 定义模块上下文  hello模块不涉及 */
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


/* 定义模块 */
ngx_module_t ngx_http_hello_module = {
	NGX_MODULE_V1,
	&ngx_http_hello_module_ctx, // 模块上下文
	ngx_http_hello_commands, // 模块命令
	NGX_HTTP_MODULE, // 模块类型 
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
	
	// 只处理NGX_HTTP_GET 与 NGX_HTTP_HEAD 请求 
	if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) 
	{
		return NGX_HTTP_NOT_ALLOWED;
	}
	
	// 设置Content-type
	r->headers_out.content_type_len = sizeof("text/html") - 1;
	r->headers_out.content_type.len = sizeof("text/html") - 1;
	r->headers_out.content_type.data = (u_char * ) "text/html";

	// 如果请求类型为NGX_HTTP_HEAD，只发送http头
	if (r->method == NGX_HTTP_HEAD) 
	{
		r->headers_out.status = NGX_HTTP_OK;
		r->headers_out.content_length_n = sizeof(ngx_hello_string) - 1;
		return ngx_http_send_header(r);
	}
	// 从内存池申请一块ngx_buf 
	b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
	if (b == NULL) 
	{
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	

	// 设置ngx buf
	b->pos = ngx_hello_string;
	b->last = ngx_hello_string + sizeof(ngx_hello_string) - 1;
	b->memory = 1; // ngx buf禁止修改
	b->last_buf = 1; // 表示该ngx buf 为buf chain最后一块缓存
	
	// 将ngx buf 加入 buf chain 
	out.buf = b;
	out.next = NULL;
	
	// 设置状态行 
	r->headers_out.status = NGX_HTTP_OK;
	// 设置content长度
	r->headers_out.content_length_n = sizeof(ngx_hello_string) - 1;

	// 发送回应http头
	rc = ngx_http_send_header(r);
	if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) 
	{
		return rc;
	}
	
	// 发送回应body 
	return ngx_http_output_filter(r, &out);
	
}


/* 设置commmand 对应的处理handler */
static char *
ngx_http_hello(ngx_conf_t * cf, ngx_command_t * cmd, void * conf)
{
	ngx_http_core_loc_conf_t * clcf;
	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_hello_handler; 
	return NGX_CONF_OK;
}





