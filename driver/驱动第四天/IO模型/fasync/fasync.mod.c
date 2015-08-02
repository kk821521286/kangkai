#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x6a91b2bc, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x88ea4119, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x68dfc59f, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xf2975f0b, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x809950d5, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0x2e60bace, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x2f287f0d, __VMLINUX_SYMBOL_STR(copy_to_user) },
	{ 0x4fa7a290, __VMLINUX_SYMBOL_STR(kill_fasync) },
	{ 0xe45f60d8, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x75bb675a, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x622fa02a, __VMLINUX_SYMBOL_STR(prepare_to_wait) },
	{ 0x4292364c, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xc8b57c27, __VMLINUX_SYMBOL_STR(autoremove_wake_function) },
	{ 0xe2491fc0, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xc4554217, __VMLINUX_SYMBOL_STR(up) },
	{ 0xdd1a2871, __VMLINUX_SYMBOL_STR(down) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x40e611da, __VMLINUX_SYMBOL_STR(fasync_helper) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "80085F691536E580DF31647");
