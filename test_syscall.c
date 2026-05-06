// test_syscall.c

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define SYS_my_add             549
#define SYS_my_task_info       550
#define SYS_my_get_comm        551
#define SYS_my_print_user_msg  552

static void test_my_add(void)
{
	long ret;

	errno = 0;
	ret = syscall(SYS_my_add, 10, 20);

	if (ret < 0) {
		printf("[user] my_add failed: ret=%ld errno=%d %s\n",
		       ret, errno, strerror(errno));
		return;
	}

	printf("[user] my_add ret = %ld\n", ret);
}

static void test_my_task_info(void)
{
	long ret;

	errno = 0;
	ret = syscall(SYS_my_task_info);

	if (ret < 0) {
		printf("[user] my_task_info failed: ret=%ld errno=%d %s\n",
		       ret, errno, strerror(errno));
		return;
	}

	printf("[user] my_task_info ret pid = %ld\n", ret);
}

static void test_my_get_comm_ok(void)
{
	long ret;
	char comm[16];

	memset(comm, 0, sizeof(comm));

	errno = 0;
	ret = syscall(SYS_my_get_comm, comm, sizeof(comm));

	if (ret < 0) {
		printf("[user] my_get_comm failed: ret=%ld errno=%d %s\n",
		       ret, errno, strerror(errno));
		return;
	}

	printf("[user] my_get_comm ret = %ld, comm = %s\n", ret, comm);
}

static void test_my_get_comm_small_len(void)
{
	long ret;
	char comm[4];

	memset(comm, 0, sizeof(comm));

	errno = 0;
	ret = syscall(SYS_my_get_comm, comm, sizeof(comm));

	if (ret < 0) {
		printf("[user] my_get_comm small len failed as expected: ret=%ld errno=%d %s\n",
		       ret, errno, strerror(errno));
		return;
	}

	printf("[user] unexpected success: ret=%ld comm=%s\n", ret, comm);
}

static void test_my_get_comm_bad_pointer(void)
{
	long ret;

	errno = 0;
	ret = syscall(SYS_my_get_comm, (char *)0x1, 16);

	if (ret < 0) {
		printf("[user] my_get_comm bad pointer failed as expected: ret=%ld errno=%d %s\n",
		       ret, errno, strerror(errno));
		return;
	}

	printf("[user] unexpected success: ret=%ld\n", ret);
}

static void test_my_print_user_msg_ok(void)
{
	long ret;
	const char *msg = "hello kernel";

	errno = 0;
	ret = syscall(SYS_my_print_user_msg, msg, strlen(msg));

	if (ret < 0) {
		printf("[user] my_print_user_msg failed: ret=%ld errno=%d %s\n",
		       ret, errno, strerror(errno));
		return;
	}

	printf("[user] my_print_user_msg ret = %ld\n", ret);
}

static void test_my_print_user_msg_bad_pointer(void)
{
	long ret;

	errno = 0;
	ret = syscall(SYS_my_print_user_msg, (char *)0x1, 12);

	if (ret < 0) {
		printf("[user] my_print_user_msg bad pointer failed as expected: ret=%ld errno=%d %s\n",
		       ret, errno, strerror(errno));
		return;
	}

	printf("[user] unexpected success: ret=%ld\n", ret);
}

int main(void)
{
	printf("===== syscall lab test start =====\n");

	test_my_add();
	test_my_task_info();
	test_my_get_comm_ok();
	test_my_get_comm_small_len();
	test_my_get_comm_bad_pointer();
	test_my_print_user_msg_ok();
	test_my_print_user_msg_bad_pointer();

	printf("===== syscall lab test end =====\n");

	return 0;
}
