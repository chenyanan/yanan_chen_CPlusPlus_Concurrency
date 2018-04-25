//
//  ThreadSafeQueue.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/8/5.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "ThreadSafeQueue.h"
#import "AppDelegate.h"

extern void threadsafe_queue_practice();

@interface ThreadSafeQueue ()

@end

@implementation ThreadSafeQueue

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"ThreadSafeQueue";
    
    threadsafe_queue_practice();
    
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
