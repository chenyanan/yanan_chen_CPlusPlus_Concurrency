//
//  b.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/23.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "B.h"
#include <thread>
#include <string>
#include <vector>

typedef int widget_id;

extern void thread_detach_use_some_local_object();
extern void thread_join_in_catch_block_when_throw_before_default_join_call();
extern void use_thread_guard();
extern void do_background_work();
extern void edit_document(const std::string& filename);
extern std::string get_filename_from_user();
extern void oops(int some_param);
extern void not_oops(int some_param);
extern void oops_again(widget_id w);
extern void not_oops_again(widget_id w);
extern void use_unique_ptr_as_parameter();
extern void some_thread_move_operations();
extern std::thread return_temporary_thread();
extern std::thread return_local_thread();
extern void function_call_function_use_thread_as_parameter();
extern void function_use_scoped_thread();
extern void produce_multiple_threads();
extern void thread_id_equal();
extern void print_thread_id();

@interface B ()

@end

@implementation B

- (void)viewDidLoad {
    [super viewDidLoad];
    
    thread_detach_use_some_local_object();
    thread_join_in_catch_block_when_throw_before_default_join_call();
    use_thread_guard();
    do_background_work();
    edit_document(get_filename_from_user());
    oops(100);
    not_oops(100);
    oops_again(100);
    not_oops_again(100);
    use_unique_ptr_as_parameter();
    some_thread_move_operations();
    std::thread t1 = return_temporary_thread();
    t1.join();
    return_local_thread().join();
    function_call_function_use_thread_as_parameter();
    function_use_scoped_thread();
    produce_multiple_threads();
    thread_id_equal();
    print_thread_id();
    
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
