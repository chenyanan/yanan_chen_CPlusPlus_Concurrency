//
//  SayConcurrentHello.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/24.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "SayConcurrentHello.h"
#import "AppDelegate.h"

extern void say_concurrent_hello();

@interface SayConcurrentHello ()

@end

@implementation SayConcurrentHello

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"SayConcurrentHello";
    
    say_concurrent_hello();
    
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
