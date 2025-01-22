#pragma once
class Camera final
{
public:
	explicit Camera(float width, float height);
	~Camera() = default;
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(Camera&& other) = delete;
	
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target) const;
	
private:
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;

	Point2f Track(const Rectf& target) const;
	void Clamp(Point2f& bottomLeftPos) const;
};