//
//  ToSharedFuture.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/6.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "ToSharedFuture.h"
#import "AppDelegate.h"

extern void future_to_shared_future();
extern void promise_to_shared_future_implicit();
extern void promise_to_shared_future_auto();
extern void multiple_shared_future();

@interface ToSharedFuture ()

@end

@implementation ToSharedFuture

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"ToSharedFuture";
    
    future_to_shared_future();
    promise_to_shared_future_implicit();
    promise_to_shared_future_auto();
    multiple_shared_future();
    
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
