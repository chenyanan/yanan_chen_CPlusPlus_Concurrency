//
//  AtomicBool.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/21.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "AtomicBool.h"
#import "AppDelegate.h"

extern void create_atomic_bool_and_assign_to_it();
extern void atomic_bool_load_store_exchange();
extern void atomic_bool_compare_exchange_weak();
extern void atomic_bool_compare_exchange_weak_with_two_sync_parameter();

@interface AtomicBool ()

@end

@implementation AtomicBool

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"AtomicBool";
    
    create_atomic_bool_and_assign_to_it();
    atomic_bool_load_store_exchange();
    atomic_bool_compare_exchange_weak();
    atomic_bool_compare_exchange_weak_with_two_sync_parameter();
    
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
