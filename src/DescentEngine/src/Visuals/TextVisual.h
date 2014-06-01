#pragma once

#include <boost/noncopyable.hpp>
#include <vector>
#include "../Texture.h"
#include "../TextureRef.h"
#include "../VectorTypes.h"
#include "ScreenTransform.h"
#include "LayerPriority.h"
#include "VisualBase.h"

class TextVisual: public VisualBase {
public:
	enum {
		TextureCoordsSize = (4 * 2), VertexBufferSize = (4 * 3)
	};

	TextVisual(ScreenTransform const& trans, TexturePtr tex, Vector2 const& intialPosition, std::string text) :
			m_textureRef(tex), m_text(text), m_ingame(false) {
		update(trans, intialPosition);
	}

	void update(ScreenTransform const& trans, Vector2 const& pos) {

		//logging::Info() << "Generating Text for " << m_text;

		m_lastPosition = pos;

		m_uvBuffer.clear();
		m_vertexBuffer.clear();

		const float zPos = 1.0f;
		const Vector2 fontSpacing(0.8f, 1.0f);

		for (unsigned int i = 0; i < m_text.size(); i++) {
			Vector2 vertex_up_left = Vector2(pos.x() + i * fontSpacing.x(),
					(pos.y() + fontSpacing.y()) * getSizeFactor());
			Vector2 vertex_bottom_right = Vector2(
					(pos.x() + i * fontSpacing.x() + fontSpacing.x()) * getSizeFactor(), pos.y());

			vertex_up_left = trans.vectorToScreen(vertex_up_left, false, false);
			vertex_bottom_right = trans.vectorToScreen(vertex_bottom_right, false, false);

			// Trig 1
			m_vertexBuffer.push_back(vertex_up_left.x());
			m_vertexBuffer.push_back(-vertex_up_left.y());
			m_vertexBuffer.push_back(zPos);

			m_vertexBuffer.push_back(vertex_up_left.x());
			m_vertexBuffer.push_back(-vertex_bottom_right.y());
			m_vertexBuffer.push_back(zPos);

			m_vertexBuffer.push_back(vertex_bottom_right.x());
			m_vertexBuffer.push_back(-vertex_up_left.y());
			m_vertexBuffer.push_back(zPos);

			// Trig 2
			m_vertexBuffer.push_back(vertex_bottom_right.x());
			m_vertexBuffer.push_back(-vertex_bottom_right.y());
			m_vertexBuffer.push_back(zPos);

			m_vertexBuffer.push_back(vertex_bottom_right.x());
			m_vertexBuffer.push_back(-vertex_up_left.y());
			m_vertexBuffer.push_back(zPos);

			m_vertexBuffer.push_back(vertex_up_left.x());
			m_vertexBuffer.push_back(-vertex_bottom_right.y());
			m_vertexBuffer.push_back(zPos);

			char character = m_text[i];
			const float uv_x = (character % 16) / 16.0f;
			const float uv_y = (character / 16) / 16.0f;
			const float sizeChar = 1.0f / 16.0f;
			Vector2 uv_up_left(uv_x, uv_y);
			Vector2 uv_up_right(uv_x + sizeChar, uv_y);
			Vector2 uv_down_right(uv_x + sizeChar, uv_y + sizeChar);
			Vector2 uv_down_left(uv_x, uv_y + sizeChar);

			uv_up_left.addToList(m_uvBuffer);
			uv_down_left.addToList(m_uvBuffer);
			uv_up_right.addToList(m_uvBuffer);

			uv_down_right.addToList(m_uvBuffer);
			uv_up_right.addToList(m_uvBuffer);
			uv_down_left.addToList(m_uvBuffer);

		}

	}

	TexturePtr getTexture() const {
		return m_textureRef;
	}

	size_t getCharacterCount() const {
		return m_text.size();
	}

	std::vector<GLfloat> const& getVertexBuffer() const {
		return m_vertexBuffer;
	}

	std::vector<GLfloat> const& getTextureBuffer() const {
		return m_uvBuffer;
	}

	void reset() {
	}

	void setText(std::string const& text, ScreenTransform const& trans) {
		m_text = text;
		update(trans, m_lastPosition);
	}

	bool isIngame() const {
		return m_ingame;
	}

	void setIngame(bool ig) {
		m_ingame = ig;
	}

	//update must be called !
	void setSizeFactor(float f) {
		m_sizeFactor = f;
	}

	float getSizeFactor() const {
		return m_sizeFactor;
	}

private:
	TexturePtr m_textureRef;
	Vector2 m_lastPosition;
	std::vector<GLfloat> m_vertexBuffer;
	std::vector<GLfloat> m_uvBuffer;

	std::string m_text;
	bool m_ingame;
	float m_sizeFactor = 1.0f;
};

