//
//  TD.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/25.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "TD.h"
#import "AppDelegate.h"

static const NSInteger _MM_SIZE = 17;

static const char* _MM[_MM_SIZE] =
{
    "D",
    "WaitForFlag",
    "DataPrepareDataProcess",
    "ThreadSafeQueue",
    "GetReturnValueFromAnotherThread",
    "MultipeAsync",
    "PackageTask",
    "PromiseAndFuture",
    "SquareRootOutOfRangeNormal",
    "SquareRootOutOfRangeFuture",
    "PromiseSetException",
    "PromiseDestruction",
    "PackageTaskDestruction",
    "ToSharedFuture",
    "Clock",
    "QuickSort",
    "MessageQueue"
};

@interface TD () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *tableView;
@end

@implementation TD

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"Chapter Four";
    
    [self tableView];
    
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - get & set
- (UITableView *)tableView
{
    if (!_table)
    {
        _table = [[UITableView alloc] initWithFrame:CGRectIntegral(CGRectMake(0, navigation_height, CGRectGetWidth([UIScreen mainScreen].bounds), CGRectGetHeight([UIScreen mainScreen].bounds) - tabbar_height - navigation_height))
                                              style:UITableViewStylePlain];
        [_table registerClass:[UITableViewCell class] forCellReuseIdentifier:NSStringFromClass([UITableViewCell class])];
        [_table setSeparatorStyle:UITableViewCellSeparatorStyleSingleLine];
        [_table setBackgroundColor:[UIColor whiteColor]];
        [_table setShowsVerticalScrollIndicator:NO];
        [_table setDataSource:self];
        [_table setDelegate:self];
        
        [self.view addSubview:_table];
    }
    return _table;
}

#pragma mark - UITableViewDelegate & UITableViewDataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _MM_SIZE;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60.0f;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return (UITableViewCell *)[tableView dequeueReusableCellWithIdentifier:NSStringFromClass([UITableViewCell class]) forIndexPath:indexPath];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
    
    UIViewController *aa = [[NSClassFromString(@(_MM[indexPath.row])) alloc] init];
    [self.navigationController pushViewController:aa animated:YES];
}

- (void)tableView:(UITableView *)tableView willDisplayCell:(UITableViewCell *)cell forRowAtIndexPath:(NSIndexPath *)indexPath
{
    cell.textLabel.text = @(_MM[indexPath.row]);
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
