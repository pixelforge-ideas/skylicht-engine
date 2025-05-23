/*
!@
MIT License

Copyright (c) 2021 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the Rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/

#include "pch.h"
#include "CPropertyController.h"
#include "CSceneController.h"
#include "CGUIDesignController.h"

#include "Activator/CEditorActivator.h"
#include "Editor/Components/CComponentEditor.h"
#include "Editor/Components/Default/CDefaultEditor.h"

#include "Reactive/CObserver.h"

namespace Skylicht
{
	namespace Editor
	{
		IMPLEMENT_SINGLETON(CPropertyController);

		CPropertyController::CPropertyController()
		{
			m_particleEditor = new CParticleEditor();
		}

		CPropertyController::~CPropertyController()
		{
			delete m_particleEditor;
		}

		void CPropertyController::setProperty(CSelectObject* object)
		{
			CSceneController* sceneController = CSceneController::getInstance();
			CGUIDesignController* guiDesignController = CGUIDesignController::getInstance();

			CScene* scene = sceneController->getScene();

			CSpaceProperty* spaceProperty = (CSpaceProperty*)CEditor::getInstance()->getWorkspaceByName(L"Property");
			if (spaceProperty == NULL)
				return;

			if (object != NULL)
			{
				if (object->getType() == CSelectObject::GameObject)
				{
					clearProperty();

					CGameObject* obj = scene->searchObjectInChildByID(object->getID().c_str());
					if (obj != NULL)
					{
						// Name and icon
						spaceProperty->setIcon(GUI::ESystemIcon::Res3D);
						spaceProperty->setLabel(obj->getName());


						// When change the name
						object->addObserver(new CObserver([spaceProperty, obj](ISubject* subject, IObserver* from)
							{
								spaceProperty->setLabel(obj->getName());
							}), true);

						setGameObjectProperty(spaceProperty, obj);
					}
				}
				else if (object->getType() == CSelectObject::Entity)
				{
					clearProperty();

					std::wstring label = L"Entity: ";

					CEntity* entity = scene->getEntityManager()->getEntityByID(object->getID().c_str());
					if (entity != NULL)
					{
						CWorldTransformData* worldTransform = GET_ENTITY_DATA(entity, CWorldTransformData);
						label += CStringImp::convertUTF8ToUnicode(worldTransform->Name.c_str());

						setEntityProperty(spaceProperty, entity);
					}

					spaceProperty->setLabel(label.c_str());
				}
				else if (object->getType() == CSelectObject::GUIElement)
				{
					clearProperty();

					spaceProperty->setIcon(GUI::ESystemIcon::ObjectRect);
					std::wstring label = L"GUI: ";

					// Show GUI Property
					CCanvas* canvas = guiDesignController->getCanvas();
					CGUIElement* gui = canvas->getGUIByID(object->getID().c_str());
					if (gui)
					{
						label += gui->getNameW();

						setGUIProperty(spaceProperty, gui);
					}

					spaceProperty->setLabel(label.c_str());
				}
			}
			else
			{
				spaceProperty->setIcon(GUI::ESystemIcon::None);
				spaceProperty->setLabel(L"");
				clearProperty();
			}

			// force update layout
			spaceProperty->getWindow()->forceUpdateLayout();
		}

		void CPropertyController::clearProperty()
		{
			CSpaceProperty* spaceProperty = (CSpaceProperty*)CEditor::getInstance()->getWorkspaceByName(L"Property");
			if (spaceProperty == NULL)
				return;

			// Clear old ui
			spaceProperty->clearAllGroup();

			// Tabable
			spaceProperty->getWindow()->getCanvas()->TabableGroup.clear();
		}

		void CPropertyController::setGameObjectProperty(CSpaceProperty* space, CGameObject* obj)
		{
			// Activator
			CEditorActivator* activator = CEditorActivator::getInstance();

			// GameObject property
			CComponentEditor* editor = activator->getEditorInstance(obj->getTypeName().c_str());
			if (editor != NULL)
				space->addComponent(editor, obj);

			// Component property
			ArrayComponent& listComponents = obj->getListComponent();
			for (CComponentSystem* component : listComponents)
			{
				CComponentEditor* editor = activator->getEditorInstance(component->getTypeName().c_str());
				if (editor != NULL)
				{
					// custom editor
					space->addComponent(editor, component);
				}
				else if (component->isSerializable())
				{
					// default editor
					CDefaultEditor* defaultEditor = new CDefaultEditor();

					// check unsupported component
					CNullComponent* nullComp = dynamic_cast<CNullComponent*>(component);
					if (nullComp != NULL)
					{
						std::string name = nullComp->getName() + std::string(" (Unsupported)");
						defaultEditor->setName(name.c_str());
					}

					// set default editor
					space->addComponent(defaultEditor, component, true);
				}
			}

			// add button "add" to popup menu component
			bool isZone = dynamic_cast<CZone*>(obj) != NULL;
			if (!isZone)
			{
				GUI::CButton* button = space->addButton(L"Add");
				button->OnPress = [&, obj, space](GUI::CBase* button)
					{
						space->popupComponentMenu(obj, button);
					};
			}
		}

		void CPropertyController::setEntityProperty(CSpaceProperty* space, CEntity* entity)
		{
			CRenderMeshData* renderData = GET_ENTITY_DATA(entity, CRenderMeshData);
			if (renderData != NULL)
				space->setIcon(GUI::ESystemIcon::Poly);
			else
				space->setIcon(GUI::ESystemIcon::Axis);

			// Activator
			CEditorActivator* activator = CEditorActivator::getInstance();

			int dataCount = entity->getDataCount();
			for (int i = 0; i < dataCount; i++)
			{
				IEntityData* data = entity->getDataByIndex(i);
				if (data == NULL)
					continue;

				// GameObject property
				CEntityDataEditor* editor = activator->getEntityDataEditorInstance(data->getTypeName().c_str());
				if (editor != NULL)
					editor->initGUI(data, space);
			}
		}

		void CPropertyController::setGUIProperty(CSpaceProperty* space, CGUIElement* gui)
		{
			// Activator
			CEditorActivator* activator = CEditorActivator::getInstance();

			// GUI property
			CGUIEditor* editor = activator->getGUIEditorInstance(gui->getTypeName().c_str());
			if (editor != NULL)
				editor->initGUI(gui, space);
		}

		void CPropertyController::setParticleProperty(CSpaceProperty* spaceProperty, Particle::CParticleSerializable* ps, Particle::CParticleComponent* psComponent)
		{
			clearProperty();

			// Set icon & name object
			spaceProperty->setIcon(GUI::ESystemIcon::ParticleSystem);

			std::wstring label = psComponent->getGameObject()->getName();
			spaceProperty->setLabel(label.c_str());

			// Particle editor property
			m_particleEditor->initGUI(ps, spaceProperty);
		}
	}
}