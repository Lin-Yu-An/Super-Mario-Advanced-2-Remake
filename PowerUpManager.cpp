#include "pch.h"
#include <iostream>
#include "PowerUpManager.h"
#include "Texture.h"
//Question

PowerUpManager::PowerUpManager()
{
	Texture* pTexture{ new Texture{"Resources/Images/mushroom.png"} };

	delete pTexture;
	pTexture = nullptr;
}

PowerUpManager::~PowerUpManager()
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
		m_pItems[i] = nullptr;
	}
}

PowerUp* PowerUpManager::AddItem(const Point2f& center, PowerUp::Type type)
{
	m_pItems.push_back(new PowerUp(center, type));
	return m_pItems.back();
}

void PowerUpManager::Update(float elapsedSec)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		if (m_pItems[i] != nullptr)
		{
			m_pItems[i]->Update(elapsedSec);
		}
	}
}

void PowerUpManager::Draw() const
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		if (m_pItems[i] != nullptr)
		{
			m_pItems[i]->Draw();
		}
	}
}

size_t PowerUpManager::Size() const
{
	return m_pItems.size();
}

bool PowerUpManager::HitItem(const Rectf& rect)
{
	//CHECK IF HIT WITH RECT
	//REMOVE/DELETE IF TRUE AND RETURN TRUE
	//OTHERWISE RETURN FALSE
	//m_pItems.pop_back()
	//DO NOT FORGET TO !!!DELETE!!! IT
	//delete p;

	for (size_t idx{ 0 }; idx < m_pItems.size(); ++idx)
	{
		if (m_pItems[idx] != nullptr && m_pItems[idx]->IsOverlapping(rect))
		{
			delete m_pItems[idx];
			m_pItems[idx] = nullptr;
			m_pItems.erase(m_pItems.begin() + idx);
			return true;
		}
	}
	return false;
}
