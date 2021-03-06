//
//  firstViewController.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/23.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "firstViewController.h"
#import "AppDelegate.h"

static const NSInteger _H_SIZE = 5;
static const char* _MM[_H_SIZE] = {"TA", "TB", "TC", "TD", "TE"};
static const char* _H[_H_SIZE] = {"Chapter One", "Chapter Two", "Chapter Three", "Chapter Four", "Chapter Five"};

@interface firstViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *tableView;
@end

@implementation firstViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.title = @"C++ Concurrency";
    
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
    return _H_SIZE;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60.0f;
}

//- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
//{
//    return @(_manager->letter_at_index(section).c_str());
//}

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
    cell.textLabel.text = @(_H[indexPath.row]);
    
//    if (indexPath.row != _manager->number_of_rows_in_section(indexPath.section))
//    {
//        demoContactPersonTableViewCell *theCell = (demoContactPersonTableViewCell *)cell;
//        [theCell setModel:(const void *)_manager->get_person_contact_pointer(indexPath.section, indexPath.row)];
//    }
}

//- (void)scrollViewDidScroll:(UIScrollView *)scrollView
//{
//    CGFloat height = scrollView.bounds.size.height;
//    CGFloat contentOffsetY = scrollView.contentOffset.y;
//    CGFloat bottomOffset = scrollView.contentSize.height - contentOffsetY;
//    if (bottomOffset <= height)
//        _manager->pull_down();
//}


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
