//
//  c.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "C.h"
#import "AppDelegate.h"

class my_class {};

extern void add_to_list(int new_value);
extern bool list_contains(int value_to_find);
extern void pass_protected_data_to_function();
extern void stack_not_shared_empty_top();
extern void some_operation_on_threadsafe_stack();
extern void what_is_death_lock();
extern void swap_object_with_lock_guard();
extern void some_operation_with_hierarchicalmutext();
extern void swap_object_with_unique_lock();
extern void process_data();
extern void get_and_process_data();
extern void race_condition_come_if_operation_unprotected_by_a_lock();
extern void init_or_get_resource();
extern void init_or_get_resource_with_lock();
extern void undefined_behaviour_with_double_checked_locking();
extern void init_resource_with_call_one_and_once_flag();
extern my_class& get_my_class_instance();

@interface C ()

@end

@implementation C

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"C";
    
    add_to_list(1);
    list_contains(1);
    pass_protected_data_to_function();
    stack_not_shared_empty_top();
    some_operation_on_threadsafe_stack();
    what_is_death_lock();
    swap_object_with_lock_guard();
    some_operation_with_hierarchicalmutext();
    swap_object_with_unique_lock();
    process_data();
    get_and_process_data();
    race_condition_come_if_operation_unprotected_by_a_lock();
    init_or_get_resource();
    init_or_get_resource_with_lock();
    undefined_behaviour_with_double_checked_locking();
    init_resource_with_call_one_and_once_flag();
    get_my_class_instance();
    
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
