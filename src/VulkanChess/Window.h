#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkInstance)
struct GLFWwindow;

class Window final {
  public:
	struct Version {
		int major, minor, patch;
	};

	explicit Window(Version const& version);
	~Window();

	VkInstance& getVulkan() { return instance; }
	bool keepOpen();

  private:
	GLFWwindow* window = nullptr;
	VkInstance instance = nullptr; // NOLINT(misc-include-cleaner)
};
