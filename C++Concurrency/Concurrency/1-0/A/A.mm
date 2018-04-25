//
//  a.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/23.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "A.h"

extern void say_hello();
extern void say_concurrent_hello();

@interface A ()

@end

@implementation A

- (void)viewDidLoad {
    [super viewDidLoad];
    
    say_hello();
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
