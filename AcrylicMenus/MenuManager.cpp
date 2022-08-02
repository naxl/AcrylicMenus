#include "pch.h"
#include "MenuManager.h"
#include "ThemeHelper.h"
#include "AcrylicHelper.h"
#include "SystemHelper.h"
#include "AppearanceConfiguration.h"

using namespace AcrylicMenus;
using namespace AcrylicMenus::MenuManager;

thread_local HWND  MenuManager::g_hWnd            = NULL;
thread_local bool  MenuManager::g_bIsDarkMode     = false;
thread_local DWORD MenuManager::g_dwBkColor       = NULL;
thread_local bool  MenuManager::g_bBkColorRefresh = false;
thread_local bool  MenuManager::g_alphaFixedState = false;

void ApplyAcrylicEffect();

void MenuManager::SetCurrentMenu(HWND hWnd)
{
	g_alphaFixedState = false;

	if (!hWnd)
	{
		g_bBkColorRefresh = false;
	}
	else if (!g_bBkColorRefresh)
	{
		RefreshCurrentMenuBackground(ThemeHelper::IsMenuUseDarkMode(hWnd), NULL);
	}

	g_hWnd = hWnd;

	ApplyAcrylicEffect();
}

void MenuManager::RefreshCurrentMenuBackground(bool bIsDarkMode, DWORD dwBkColor)
{
	if (ACRYLIC_TINT_AUTO)
	{
		if (dwBkColor)
		{
			g_dwBkColor = dwBkColor;
			g_bBkColorRefresh = true;
		}
		else
		{
			ThemeHelper::GetMenuFillColor(
				g_bIsDarkMode ? L"ImmersiveStartDark::Menu" : L"ImmersiveStart::Menu",
				&g_dwBkColor
			);
		}
	}
	if (g_bIsDarkMode != bIsDarkMode)
	{
		ApplyAcrylicEffect();
	}
	g_bIsDarkMode = bIsDarkMode;
}

void ApplyAcrylicEffect()
{
	if (SystemHelper::g_bIsWindows11)
	{
		AcrylicHelper::SetAcrylicEffect(
			g_hWnd,
			g_bIsDarkMode ? WIN11_LUMINOSITY_TINT_DARK : WIN11_LUMINOSITY_TINT_LIGHT
		);
	}
	else
	{
		AcrylicHelper::SetAcrylicEffect(
			g_hWnd,
			ACRYLIC_TINT_AUTO ? g_dwBkColor : (g_bIsDarkMode ? ACRYLIC_TINT_DARK : ACRYLIC_TINT_LIGHT),
			g_bIsDarkMode ? ACRYLIC_OPACITY_DARK : ACRYLIC_OPACITY_LIGHT
		);
	}
}