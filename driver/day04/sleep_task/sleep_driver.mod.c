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
	{ 0x98397cc5, "module_layout" },
	{ 0x47c4a86d, "cdev_del" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x8c71be19, "cdev_add" },
	{ 0x9f4ce64, "cdev_init" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x50eedeb8, "printk" },
	{ 0x4292364c, "schedule" },
	{ 0x5f5602c6, "current_task" },
	{ 0x7734d768, "wake_up_process" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "AB143451B0D3E705D91BE58");
