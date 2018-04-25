//
//  AppDelegate.m
//  C++Concurrency
//
//  Created by chenyanan on 2017/6/23.
//  Copyright © 2017年 chenyanan. All rights reserved.
//

#import "AppDelegate.h"

#include "1_0.h"
#include "2_0.h"
#include "3_0.h"
#include "4_0.h"
#include "5_0.h"

#import "firstViewController.h"
#import "secondViewController.h"
#import "thirdViewController.h"
#import "forthViewController.h"

#import "RDVTabBar.h"
#import "RDVTabBarItem.h"
#import "RDVTabBarController.h"

#import "NavigationController.h"

const CGFloat tabbar_height = 44.0f;
const CGFloat navigation_height = 64.0f;

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    //Tab
    UIViewController *welcom = [[firstViewController alloc] init];
    UIViewController *welcomNavi = [[NavigationController alloc] initWithRootViewController:welcom];
    
    UIViewController *contact = [[secondViewController alloc] init];
    UIViewController *contactNavi = [[NavigationController alloc] initWithRootViewController:contact];
    
    UIViewController *chart = [[thirdViewController alloc] init];
    UIViewController *chartNavi = [[NavigationController alloc] initWithRootViewController:chart];
    
    UIViewController *me = [[forthViewController alloc] init];
    UIViewController *meNavi = [[NavigationController alloc] initWithRootViewController:me];
    
    RDVTabBarController *tabBarController = [[RDVTabBarController alloc] init];
    [tabBarController setViewControllers:@[welcomNavi, contactNavi, chartNavi, meNavi]];
    
    NSArray *tabBarItemImages = @[@"welcom", @"contact", @"chart", @"me"];
    
    RDVTabBar *tabBar = [tabBarController tabBar];
    
    [tabBar setFrame:CGRectMake(CGRectGetMinX(tabBar.frame), CGRectGetMinY(tabBar.frame), CGRectGetWidth(tabBar.frame), tabbar_height)];
    
    NSInteger index = 0;
    for (RDVTabBarItem *item in [[tabBarController tabBar] items])
    {
        UIImage *image = [UIImage imageNamed:tabBarItemImages[index]];
        
        item.title = tabBarItemImages[index];
        item.titlePositionAdjustment = UIOffsetMake(0, 1.0f);
        item.imagePositionAdjustment = UIOffsetMake(0, 3.0f);
        [item setFinishedSelectedImage:image withFinishedUnselectedImage:image];
        index++;
    }
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.rootViewController = tabBarController;
    [self.window makeKeyAndVisible];
    
    // Override point for customization after application launch.
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


@end
